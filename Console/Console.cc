#include "Console.hh"
#include "../Drawing/Drawing.hh"
#include "../Vendor/JSON/JSON.hpp"
#include <type_traits>
#include <fstream>
#include <filesystem>

/**
 * @brief Styling is not an issue yet, thus, I'll make stylization details global (at least, for now).
 * 
 */
//	========================================================================================================================================
constexpr static int g_iBlockSize = 18;
//	========================================================================================================================================

/**
 * @todo: Remove some redundant safety checks from Console_t::WndProc
 * 
 */

Console_t::Console_t(std::string &&strConfig)
{
	this->m_strConfig = std::move(strConfig);
}

void Console_t::Think()
{
	this->m_Animation.Set(this->m_bOpen, Animation_t{8.F, Easing::OutQuart}, Animation_t{8.F, Easing::OutQuart});

	this->m_nAvailableSuggestedOptions = 0;
	this->m_bAbleToSwitchFocus = true;

	if (this->m_bOpen && !this->m_strInputBuffer.empty() && !this->m_vecCollection.empty())
	{
		for (auto &[first, second] : this->m_vecCollection)
		{
			for (size_t i = 0; i < this->m_strInputBuffer.size(); ++i)
			{
				//	Match string thus far
				if (first.first.size() > this->m_strInputBuffer.size() && first.first[i] == this->m_strInputBuffer[i])
				{
					this->m_bAbleToSwitchFocus = false;
					first.second = true;
				}
				else
				{
					//	Failed, don't try onwards as it'd modify the state despite there being at least one incorrect character.
					first.second = false;
					break;
				}
			}

			if (first.second)
				++this->m_nAvailableSuggestedOptions;
		}
	}
}

void Console_t::WndProc(UINT u32Msg, WPARAM wParam)
{
	if (u32Msg == WM_KEYUP && wParam == VK_INSERT)
	{
		this->m_bOpen = !this->m_bOpen;
	}

	if (!this->m_bOpen)
	{
		return;
	}

	switch (u32Msg)
	{
	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_UP:
		{
			if (this->m_bInputFocus && !this->m_bAbleToSwitchFocus && this->m_nAvailableSuggestedOptions > 0)
			{
				int iValue = (this->m_nAutoSuggestedSelection - 1);
				if (iValue < 0)
					this->m_nAutoSuggestedSelection = (this->m_nAvailableSuggestedOptions - 1);
				else
					this->m_nAutoSuggestedSelection = max(0, min(iValue, this->m_nAvailableSuggestedOptions - 1));
			}
		}
		break;

		case VK_DOWN:
		{
			if (this->m_bInputFocus && !this->m_bAbleToSwitchFocus && this->m_nAvailableSuggestedOptions > 0)
				this->m_nAutoSuggestedSelection = max(0, min((this->m_nAutoSuggestedSelection + 1) % this->m_nAvailableSuggestedOptions, this->m_nAvailableSuggestedOptions - 1));
		}
		break;

		case VK_RIGHT:
		{
			SetAutoSuggestFocusIfPossible();
		}
		break;
		default:
		{
		}
		break;
		}
		break;
	}
	break;

	case WM_CHAR:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			this->m_nAutoSuggestedSelection = 0;
			this->m_strInputBuffer.clear();
		}
		break;

		case VK_BACK:
		{
			if (this->m_bInputFocus)
			{
				//	Upon the loss of a  character, we want to reset the auto suggestion scroll count
				this->m_nAutoSuggestedSelection = 0;

				if (!this->m_strInputBuffer.empty())
					this->m_strInputBuffer.pop_back();
			}
		}
		break;

		case VK_TAB:
		{
			if (this->m_iMaxH > 0 && (this->m_strInputBuffer.empty() || this->m_bAbleToSwitchFocus))
				this->m_bInputFocus = !this->m_bInputFocus;
			else
			{
				if (this->m_nAvailableSuggestedOptions > 0)
					this->m_nAutoSuggestedSelection = max(0, min((this->m_nAutoSuggestedSelection + 1) % this->m_nAvailableSuggestedOptions, this->m_nAvailableSuggestedOptions - 1));
			}
		}
		break;

		case VK_RETURN:
		{
			bool bState = SetAutoSuggestFocusIfPossible();
			if (!bState)
			{
				ProcessBuffer();
			}
		}
		break;

		default:
		{
			if (this->m_bInputFocus)
			{
				//	Upon a new character, we want to reset the auto suggestion scroll count
				//	as there may be less autosuggested options
				this->m_nAutoSuggestedSelection = 0;

				char chInput = (char)wParam;
				if (chInput > 0 && chInput < 0xFF)
					this->m_strInputBuffer.push_back(chInput);
			}
		}
		break;
		}
	}
	break;

	case WM_MOUSEWHEEL:
	{
		if (!this->m_bInputFocus)
		{
			int iDelta = (GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			if (this->m_iMaxH > this->m_iH)
				this->m_iScrollage = max(0, min(this->m_iMaxH - this->m_iH, this->m_iScrollage + (iDelta < 0 ? -7 : 7)));
		}
	}
	break;
	}
}

void Console_t::Draw(Drawing_t *pDraw) const
{
	if (this->m_Animation.Get() > 0.F)
	{
		static const ImFont *pFont = pDraw->GetFont(HASH("Terminus"));

		int iW = pDraw->m_iScreenW;
		//	Ugly... fix later
		this->m_iH = min(pDraw->m_iScreenH / 6, this->m_iMaxH);

		//	Drop animation
		int iAnimY = this->m_iH - (this->m_iH * this->m_Animation.Get());

		//	Console Buffer Zone
		if (this->m_iH > 0)
		{
			pDraw->PushClip(0, -iAnimY, iW, this->m_iH);

			//	Draw Background
			pDraw->Draw(Rectangle_t(0, -iAnimY, iW, this->m_iH, Color_t(36, 37, 42, 102)));

			for (size_t i = 0; i < this->m_deqConsoleBuffer.size(); ++i)
			{
				const Color_t &color = (this->m_deqConsoleBuffer[i][0] == '!') ? Color_t(255, 0, 0, 255) : (this->m_bInputFocus ? Color_t(190, 190, 190, 255) : Color_t(255, 255, 255, 255));

				const Text_t &text = Text_t(5, this->m_iH - (g_iBlockSize * (i + 1)) + this->m_iScrollage - iAnimY, std::move(this->m_deqConsoleBuffer[i]), pFont, 15.F, color);

				//	Rendering optimization
				if (text.m_iY < (-g_iBlockSize))
					continue;

				if (text.m_iY > this->m_iH + g_iBlockSize)
					continue;

				pDraw->Draw(text);
			}

			pDraw->PopClip();
		}

		//	Input Zone
		const Color_t &color = this->m_bInputFocus ? Color_t(32, 32, 32, 142) : Color_t(18, 18, 18, 142);
		const Color_t &textColor = this->m_bInputFocus ? Color_t(255, 255, 255, 255) : Color_t(190, 190, 190, 255);
		pDraw->Draw(Rectangle_t(0, this->m_iH - iAnimY, iW, g_iBlockSize + 2, color));

		Text_t text = Text_t(5, this->m_iH + (g_iBlockSize / 2) - iAnimY, std::move(this->m_strInputBuffer), pFont, 15.F, textColor);
		text.m_iY -= text.m_iH / 2;

		int iOldW = text.m_iW;

		pDraw->Draw(text);

		//	Text Cursor
		if (this->m_bInputFocus)
		{
			int iLevel = (g_iBlockSize - 6) / 2;
			pDraw->Draw(RectangleGradient_t(6 + iOldW, this->m_iH + 4 - iAnimY, 1, iLevel, Color_t(255, 255, 255, 30), textColor, false));
			pDraw->Draw(RectangleGradient_t(6 + iOldW, this->m_iH + 4 + iLevel - iAnimY, 1, iLevel, textColor, Color_t(255, 255, 255, 30), false));
		}

		//	Input Buffer
		int iCount = 0;
		if (!this->m_strInputBuffer.empty())
		{
			for (const auto &[first, second] : this->m_vecCollection)
			{
				if (!first.second)
					continue;

				int iPad = ((iCount % 2 == 0) ? 0 : 8) + ((this->m_nAutoSuggestedSelection == iCount) ? 32 : 0);

				const Color_t &color = Color_t(36 + iPad, 36 + iPad, 36 + iPad, 142);

				Text_t text = Text_t(5, this->m_iH + g_iBlockSize + 2 + (g_iBlockSize * iCount) - iAnimY, std::move(first.first), pFont, 15.F, Color_t(255, 255, 255, 255));
				text.m_iY += 1;

				const Text_t &tag = Text_t(text.m_iX + text.m_iW + 10, text.m_iY - iAnimY, second.first ? "(Callback)" : "(Identifier)", pFont, 15.F, Color_t(220, 220, 220, 255));

				pDraw->Draw(Rectangle_t(0, this->m_iH + g_iBlockSize + 2 + (g_iBlockSize * iCount) - iAnimY, text.m_iW + tag.m_iW + 20, g_iBlockSize, color));

				pDraw->Draw(tag);
				pDraw->Draw(text);

				++iCount;
			}
		}
	}
}

void Console_t::AddCallback(std::string &&strName, Callback_t &&method)
{
	const Hash_t hHash = RT_HASH(strName.data());
	//	Store string form of the callback for auto-suggest
	//	Also apply characteristic.
	//	String not suggestible by default.
	this->m_vecCollection.emplace_back(std::make_pair(std::move(strName), false), std::make_pair(true, hHash));

	//	Assign callback to hash.
	this->m_umCallbacks[hHash] = std::move(method);
}

void Console_t::AddIdentifier(std::string &&strName, Config_t defaultValue)
{
	const Hash_t hHash = RT_HASH(strName.data());
	//	Store string form of the identifier for auto-suggest
	//	Also apply characteristic.
	//	String not suggestible by default.
	this->m_vecCollection.emplace_back(std::make_pair(std::move(strName), false), std::make_pair(false, hHash));

	//	Type will be processable by holds_alternative.
	this->m_umIdentifiers[hHash] = defaultValue;
}

void Console_t::WriteToBuffer(std::string &&strText)
{
	this->m_deqConsoleBuffer.emplace_front(std::move(strText));
	this->m_iMaxH += g_iBlockSize;

	//	Make user notice last write
	this->m_iScrollage = 0;
}

void Console_t::ProcessBuffer()
{
	//	Nothing to process.
	if (this->m_strInputBuffer.empty())
		return;

	std::string strValidatedComputeString = this->m_strInputBuffer;

	const size_t iFirstOfSpace = this->m_strInputBuffer.find_first_of(' ');
	//	Staying intentionally one short so we skip the said character.
	if (iFirstOfSpace != std::string::npos)
		strValidatedComputeString = strValidatedComputeString.substr(0, iFirstOfSpace);

	const Hash_t hHash = RT_HASH(strValidatedComputeString.data());
	if (this->m_umCallbacks.contains(hHash))
	{
		WriteToBuffer("> " + this->m_strInputBuffer);

		//	Execute callback
		bool bState = this->m_umCallbacks[hHash](this);
		if (!bState)
		{
			goto ERROR_LABEL;
		}
	}
	else if (this->m_umIdentifiers.contains(hHash))
	{
		Config_t &identifierValue = this->m_umIdentifiers[hHash];
		//	Identifier found, no value anywho
		if (iFirstOfSpace == std::string::npos)
		{
			if (std::holds_alternative<bool>(identifierValue))
			{
				WriteToBuffer(strValidatedComputeString + ": " + (std::get<bool>(identifierValue) ? "true" : "false"));
			}
			else if (std::holds_alternative<int>(identifierValue))
			{
				WriteToBuffer(strValidatedComputeString + ": " + std::to_string(std::get<int>(identifierValue)));
			}
			else if (std::holds_alternative<Color_t>(identifierValue))
			{
				const Color_t &colIdentifierValue = std::get<Color_t>(identifierValue);
				WriteToBuffer(strValidatedComputeString + ": (" + std::to_string(colIdentifierValue.m_u8R) + ", " + std::to_string(colIdentifierValue.m_u8G) + ", " + std::to_string(colIdentifierValue.m_u8B) + ", " + std::to_string(colIdentifierValue.m_u8A) + ")");
			}
		}
		//	Identifier found, spacing found
		else
		{
			//	Found something that could represent a value
			const size_t iValueStart = this->m_strInputBuffer.find_first_not_of(' ', iFirstOfSpace + 1);
			//	Keywourd: 'could'
			if (iValueStart == std::string::npos)
				goto ERROR_LABEL;

			const std::string &strDeducedValue = this->m_strInputBuffer.substr(iValueStart);

			if (std::holds_alternative<bool>(identifierValue))
			{
				identifierValue = !!min(std::atoi(strDeducedValue.c_str()), 1);

				WriteToBuffer(strValidatedComputeString + " -> " + (std::get<bool>(identifierValue) ? "true" : "false"));
			}
			else if (std::holds_alternative<int>(identifierValue))
			{
				identifierValue = std::atoi(strDeducedValue.c_str());

				WriteToBuffer(strValidatedComputeString + " -> " + std::to_string(std::get<int>(identifierValue)));
			}
			//	HAX ALERT WEEWOOWEEEWOOO
			else if (std::holds_alternative<Color_t>(identifierValue))
			{
				if (strDeducedValue.size() != 8)
					goto ERROR_LABEL;

				uint32_t u32Value = 0;
				std::stringstream sstrConvertToHex;
				sstrConvertToHex << std::hex << strDeducedValue;
				sstrConvertToHex >> u32Value;

				*(uint32_t *)(&std::get<Color_t>(identifierValue)) = Helveta::detail::EndiannessSwap32bit(u32Value);

				const Color_t &colIdentifierValue = std::get<Color_t>(identifierValue);
				WriteToBuffer(strValidatedComputeString + " -> (" + std::to_string(colIdentifierValue.m_u8R) + ", " + std::to_string(colIdentifierValue.m_u8G) + ", " + std::to_string(colIdentifierValue.m_u8B) + ", " + std::to_string(colIdentifierValue.m_u8A) + ")");
			}
		}
	}
	else
	{
	ERROR_LABEL:
		WriteToBuffer("!Failed executing the following: \"" + this->m_strInputBuffer + "\".");
	}

	this->m_strInputBuffer.clear();
}

bool Console_t::SetAutoSuggestFocusIfPossible()
{
	//	Set auto suggest condition
	if (this->m_bInputFocus && !this->m_bAbleToSwitchFocus && this->m_nAvailableSuggestedOptions > 0)
	{
		//	Find Nth compliant suggestion
		int iCount = 0;
		for (const auto &[first, second] : this->m_vecCollection)
		{
			if (!first.second)
				continue;

			if (iCount == this->m_nAutoSuggestedSelection)
			{
				this->m_strInputBuffer = first.first;
				this->m_nAutoSuggestedSelection = 0;
				break;
			}

			++iCount;
		}

		//	Focus on our current value
		this->m_strInputBuffer.push_back(' ');

		return true;
	}

	return false;
}

void Console_t::SaveConfig() const
{
	nlohmann::json jsonConfig;

	//	Traverse collection for identifiers
	for (const auto &[first, second] : this->m_vecCollection)
	{
		//	Don't process if it's a callback
		if (second.first)
			continue;

		const Config_t &value = this->m_umIdentifiers.at(second.second);
		if (std::holds_alternative<bool>(value))
			jsonConfig[first.first] = {{"identifier", value.index()}, {"value", std::get<bool>(value)}};
		else if (std::holds_alternative<int>(value))
			jsonConfig[first.first] = {{"identifier", value.index()}, {"value", std::get<int>(value)}};
		else if (std::holds_alternative<Color_t>(value))
		{
			const Color_t &color = std::get<Color_t>(value);
			jsonConfig[first.first] = {{"identifier", value.index()}, {"value_r", color.m_u8R}, {"value_g", color.m_u8G}, {"value_b", color.m_u8B}, {"value_a", color.m_u8A}};
		}
	}

	std::ofstream ofConfigStream(this->m_strConfig);
	ofConfigStream << jsonConfig << std::endl;
	ofConfigStream.close();
}

bool Console_t::LoadConfig()
{
	if (!(std::filesystem::exists(std::filesystem::current_path() / this->m_strConfig)))
		return false;

	std::ifstream ifConfigStream(this->m_strConfig);

	const nlohmann::json &jsonConfig = nlohmann::json::parse(ifConfigStream);
	//	Traverse collection for identifiers
	for (const auto &[first, second] : this->m_vecCollection)
	{
		//	Don't process if it's a callback
		if (second.first)
			continue;

		if (jsonConfig.contains(first.first))
		{
			const nlohmann::json &entry = jsonConfig[first.first];
			if (entry.empty())
			{
				continue;
			}

			switch (entry["identifier"].get<int>())
			{
			case 0:
			{
				const nlohmann::json &value = entry["value"];
				this->m_umIdentifiers[second.second] = value.get<bool>();
			}
			break;
			case 1:
			{
				const nlohmann::json &value = entry["value"];
				this->m_umIdentifiers[second.second] = value.get<int>();
			}
			break;
			case 2:
			{
				const Color_t &color = Color_t(entry["value_r"].get<uint8_t>(), entry["value_g"].get<uint8_t>(), entry["value_b"].get<uint8_t>(), entry["value_a"].get<uint8_t>());
				this->m_umIdentifiers[second.second] = color;
			}
			break;
			}
		}
	}

	ifConfigStream.close();
	return true;
}