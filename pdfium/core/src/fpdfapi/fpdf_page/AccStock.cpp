#include "../../../include/fpdfapi/fpdf_page.h"
#include "../../../include/fpdfapi/fpdf_module.h"
#include "../../../include/fpdfapi/fpdf_serial.h"
#include "pageint.h"


#include <unordered_map>
#include <string>
#include <vector>
#define REQUIRE_PARAMS(count) if (m_ParamCount != count) { m_bAbort = TRUE; return; }

//AccStock::AccStock()
//{
//}
//AccStock::~AccStock()
//{
//}
//void AccStock::AddFont(CPDF_Font* fontToAdd)
//{
//	auto size = sizeof(fontToAdd);
//
//	//auto fontKey = std::string(key);
//	//if (font_hm.find(fontKey) != font_hm.end())
//	//{
//	//	return (font_hm[fontKey]);
//	//}
//	//printf("%s\n", key);
//	//font_hm[fontKey] = pFontStock;
//	//CPDF_Font font ;
//}


bool Acc_GetCharBBox(FX_DWORD charcode, FX_RECT& rect, int level)
{
	switch (charcode) 
	{
	case 100:rect = FX_RECT(35, 708, 500, 0); break;
	case 101:rect = FX_RECT(23, 142, 232, 11); break;
	case 102:rect = FX_RECT(41, 142, 209, 0); break;
	case 103:rect = FX_RECT(35, 708, 507, 11); break;
	case 104:rect = FX_RECT(69, 142, 209, 0); break;
	case 105:rect = FX_RECT(44, 721, 513, 11); break;
	case 106:rect = FX_RECT(13, 708, 654, 0); break;
	case 107:rect = FX_RECT(13, 708, 474, 0); break;
	case 108:rect = FX_RECT(50, 708, 474, 0); break;
	case 109:rect = FX_RECT(13, 530, 602, 0); break;
	case 110:rect = FX_RECT(50, 530, 474, 0); break;
	case 111:rect = FX_RECT(54, 85, 196, 0); break;
	case 112:rect = FX_RECT(13, 85, 196, 0); break;
	case 113:rect = FX_RECT(50, 716, 474, 6); break;
	case 114:rect = FX_RECT(54, 85, 196, 0); break;
	case 115:rect = FX_RECT(75, 530, 758, 0); break;
	case 116:rect = FX_RECT(73, 761, 507, 11); break;
	case 117:rect = FX_RECT(54, 85, 196, 133); break;
	case 14:rect = FX_RECT(123, 708, 343, 0); break;
	case 150:rect = FX_RECT(85, 783, 474, 88); break;
	case 152:rect = FX_RECT(85, 783, 474, 88); break;
	case 153:rect = FX_RECT(46, 716, 510, 6); break;
	case 155:rect = FX_RECT(86, 708, 489, 6); break;
	case 156:rect = FX_RECT(58, 708, 484, 0); break;
	case 162:rect = FX_RECT(85, 783, 474, 88); break;
	case 163:rect = FX_RECT(23, 708, 528, 0); break;
	case 165:rect = FX_RECT(58, 708, 474, 0); break;
	case 166:rect = FX_RECT(46, 716, 474, 6); break;
	case 167:rect = FX_RECT(23, 708, 528, 0); break;
	case 168:rect = FX_RECT(58, 708, 484, 0); break;
	case 169:rect = FX_RECT(23, 708, 510, 0); break;
	case 170:rect = FX_RECT(49, 716, 501, 6); break;
	case 175:rect = FX_RECT(49, 716, 501, 6); break;
	case 178:rect = FX_RECT(86, 708, 489, 6); break;
	case 179:rect = FX_RECT(77, 708, 590, 0); break;
	case 18:rect = FX_RECT(123, 708, 343, 0); break;
	case 180:rect = FX_RECT(86, 708, 489, 6); break;
	case 181:rect = FX_RECT(23, 708, 528, 0); break;
	case 182:rect = FX_RECT(49, 716, 501, 6); break;
	case 183:rect = FX_RECT(58, 708, 484, 0); break;
	case 185:rect = FX_RECT(77, 708, 590, 0); break;
	case 199:rect = FX_RECT(58, 708, 484, 0); break;
	case 200:rect = FX_RECT(86, 708, 489, 6); break;
	case 40:rect = FX_RECT(0, 0, 0, 0); break;
	case 45:rect = FX_RECT(0, 0, 0, 0); break;
	case 46:rect = FX_RECT(0, 0, 0, 0); break;
	case 69:rect = FX_RECT(40, 530, 434, 11); break;
	case 70:rect = FX_RECT(34, 530, 355, 11); break;
	case 71:rect = FX_RECT(40, 530, 416, 11); break;
	case 72:rect = FX_RECT(34, 530, 315, 0); break;
	case 73:rect = FX_RECT(40, 530, 416, 11); break;
	case 74:rect = FX_RECT(34, 530, 315, 0); break;
	case 75:rect = FX_RECT(40, 530, 416, 11); break;
	case 76:rect = FX_RECT(20, 530, 313, 0); break;
	case 77:rect = FX_RECT(40, 530, 416, 11); break;
	case 78:rect = FX_RECT(20, 530, 313, 0); break;
	case 79:rect = FX_RECT(41, 530, 416, 11); break;
	case 80:rect = FX_RECT(20, 530, 313, 0); break;
	case 81:rect = FX_RECT(41, 530, 459, 11); break;
	case 82:rect = FX_RECT(20, 665, 313, 11); break;
	case 84:rect = FX_RECT(20, 665, 313, 11); break;
	case 85:rect = FX_RECT(50, 716, 469, 0); break;
	case 86:rect = FX_RECT(49, 716, 507, 6); break;
	case 87:rect = FX_RECT(50, 708, 343, 0); break;
	case 88:rect = FX_RECT(49, 716, 507, 6); break;
	case 89:rect = FX_RECT(50, 85, 179, 0); break;
	case 9:rect = FX_RECT(123, 708, 343, 0); break;
	case 90:rect = FX_RECT(49, 85, 179, 0); break;
	case 91:rect = FX_RECT(50, 708, 469, 0); break;
	case 92:rect = FX_RECT(44, 85, 179, 0); break;
	case 93:rect = FX_RECT(23, 142, 232, 0); break;
	case 94:rect = FX_RECT(16, 142, 232, 0); break;
	case 95:rect = FX_RECT(35, 85, 179, 0); break;
	case 96:rect = FX_RECT(41, 85, 179, 0); break;
	case 97:rect = FX_RECT(23, 142, 232, 0); break;
	case 98:rect = FX_RECT(35, 142, 209, 0); break;
	case 99:rect = FX_RECT(16, 142, 209, 0); break;
	default:rect = FX_RECT(0, 0, 0, 0);
	}
	return true;
}


//bool Acc_GetCharBBox2(FX_DWORD charcode, FX_RECT& rect)
//{
//	switch (charcode) 
//	{
//	case 10:rect = FX_RECT(123, 708, 343, 0); break;
//	case 100:rect = FX_RECT(13, 85, 179, 0); break;
//	case 101:rect = FX_RECT(13, 142, 209, 0); break;
//	case 102:rect = FX_RECT(13, 85, 196, 0); break;
//	case 103:rect = FX_RECT(13, 142, 209, 0); break;
//	case 104:rect = FX_RECT(13, 85, 196, 0); break;
//	case 105:rect = FX_RECT(13, 85, 196, 0); break;
//	case 106:rect = FX_RECT(13, 85, 196, 0); break;
//	case 107:rect = FX_RECT(13, 85, 196, 0); break;
//	case 108:rect = FX_RECT(13, 85, 196, 0); break;
//	case 109:rect = FX_RECT(13, 85, 196, 0); break;
//	case 11:rect = FX_RECT(54, 85, 196, 0); break;
//	case 110:rect = FX_RECT(13, 85, 196, 0); break;
//	case 111:rect = FX_RECT(13, 85, 196, 0); break;
//	case 112:rect = FX_RECT(13, 85, 196, 0); break;
//	case 113:rect = FX_RECT(13, 85, 196, 0); break;
//	case 114:rect = FX_RECT(13, 85, 196, 0); break;
//	case 115:rect = FX_RECT(50, 85, 196, 0); break;
//	case 116:rect = FX_RECT(13, 85, 196, 0); break;
//	case 117:rect = FX_RECT(13, 85, 196, 0); break;
//	case 118:rect = FX_RECT(50, 85, 196, 0); break;
//	case 119:rect = FX_RECT(54, 85, 196, 0); break;
//	case 12:rect = FX_RECT(54, 85, 196, 0); break;
//	case 120:rect = FX_RECT(73, 530, 507, 0); break;
//	case 121:rect = FX_RECT(54, 85, 196, 11); break;
//	case 13:rect = FX_RECT(54, 85, 196, 0); break;
//	case 134:rect = FX_RECT(85, 783, 474, 88); break;
//	case 135:rect = FX_RECT(46, 716, 510, 6); break;
//	case 137:rect = FX_RECT(85, 783, 474, 88); break;
//	case 138:rect = FX_RECT(85, 708, 474, 6); break;
//	case 139:rect = FX_RECT(46, 716, 474, 6); break;
//	case 14:rect = FX_RECT(123, 708, 343, 0); break;
//	case 140:rect = FX_RECT(23, 708, 474, 0); break;
//	case 141:rect = FX_RECT(23, 708, 474, 0); break;
//	case 142:rect = FX_RECT(23, 708, 474, 0); break;
//	case 143:rect = FX_RECT(23, 708, 474, 0); break;
//	case 144:rect = FX_RECT(46, 716, 474, 6); break;
//	case 145:rect = FX_RECT(23, 708, 474, 0); break;
//	case 146:rect = FX_RECT(49, 708, 474, 0); break;
//	case 147:rect = FX_RECT(23, 708, 474, 0); break;
//	case 148:rect = FX_RECT(23, 708, 474, 0); break;
//	case 149:rect = FX_RECT(23, 708, 474, 0); break;
//	case 15:rect = FX_RECT(123, 708, 343, 0); break;
//	case 150:rect = FX_RECT(23, 708, 474, 0); break;
//	case 151:rect = FX_RECT(23, 708, 474, 0); break;
//	case 152:rect = FX_RECT(23, 708, 474, 0); break;
//	case 153:rect = FX_RECT(23, 708, 474, 0); break;
//	case 154:rect = FX_RECT(23, 708, 474, 0); break;
//	case 155:rect = FX_RECT(23, 708, 474, 0); break;
//	case 156:rect = FX_RECT(23, 708, 474, 0); break;
//	case 157:rect = FX_RECT(23, 708, 474, 0); break;
//	case 158:rect = FX_RECT(23, 708, 474, 0); break;
//	case 159:rect = FX_RECT(23, 708, 474, 0); break;
//	case 16:rect = FX_RECT(123, 708, 343, 0); break;
//	case 160:rect = FX_RECT(23, 708, 474, 0); break;
//	case 161:rect = FX_RECT(23, 708, 474, 0); break;
//	case 162:rect = FX_RECT(23, 708, 474, 0); break;
//	case 163:rect = FX_RECT(23, 708, 474, 0); break;
//	case 164:rect = FX_RECT(23, 708, 474, 0); break;
//	case 165:rect = FX_RECT(23, 708, 474, 0); break;
//	case 166:rect = FX_RECT(23, 708, 474, 0); break;
//	case 167:rect = FX_RECT(23, 708, 474, 0); break;
//	case 168:rect = FX_RECT(23, 708, 474, 0); break;
//	case 169:rect = FX_RECT(23, 708, 474, 0); break;
//	case 17:rect = FX_RECT(123, 708, 343, 0); break;
//	case 170:rect = FX_RECT(23, 708, 474, 0); break;
//	case 171:rect = FX_RECT(23, 708, 474, 0); break;
//	case 172:rect = FX_RECT(23, 708, 484, 0); break;
//	case 173:rect = FX_RECT(23, 708, 484, 0); break;
//	case 174:rect = FX_RECT(23, 708, 484, 0); break;
//	case 175:rect = FX_RECT(23, 708, 484, 0); break;
//	case 176:rect = FX_RECT(23, 708, 474, 0); break;
//	case 177:rect = FX_RECT(23, 708, 484, 0); break;
//	case 178:rect = FX_RECT(23, 708, 484, 0); break;
//	case 179:rect = FX_RECT(23, 708, 484, 0); break;
//	case 18:rect = FX_RECT(123, 708, 343, 0); break;
//	case 180:rect = FX_RECT(23, 708, 484, 0); break;
//	case 181:rect = FX_RECT(23, 708, 484, 0); break;
//	case 182:rect = FX_RECT(23, 708, 484, 0); break;
//	case 183:rect = FX_RECT(23, 708, 484, 0); break;
//	case 184:rect = FX_RECT(23, 708, 489, 0); break;
//	case 185:rect = FX_RECT(23, 708, 501, 0); break;
//	case 186:rect = FX_RECT(23, 708, 501, 0); break;
//	case 187:rect = FX_RECT(23, 708, 484, 0); break;
//	case 188:rect = FX_RECT(86, 708, 489, 6); break;
//	case 189:rect = FX_RECT(46, 708, 489, 0); break;
//	case 19:rect = FX_RECT(123, 708, 343, 0); break;
//	case 190:rect = FX_RECT(46, 708, 484, 0); break;
//	case 191:rect = FX_RECT(23, 708, 484, 0); break;
//	case 192:rect = FX_RECT(46, 708, 489, 6); break;
//	case 193:rect = FX_RECT(46, 708, 484, 0); break;
//	case 194:rect = FX_RECT(86, 708, 489, 6); break;
//	case 195:rect = FX_RECT(46, 708, 484, 0); break;
//	case 196:rect = FX_RECT(46, 708, 484, 0); break;
//	case 197:rect = FX_RECT(46, 708, 484, 0); break;
//	case 198:rect = FX_RECT(23, 708, 484, 0); break;
//	case 199:rect = FX_RECT(23, 708, 484, 0); break;
//	case 20:rect = FX_RECT(123, 708, 343, 0); break;
//	case 200:rect = FX_RECT(49, 708, 484, 0); break;
//	case 201:rect = FX_RECT(23, 708, 501, 0); break;
//	case 202:rect = FX_RECT(49, 708, 489, 6); break;
//	case 203:rect = FX_RECT(23, 708, 501, 0); break;
//	case 205:rect = FX_RECT(46, 708, 489, 6); break;
//	case 206:rect = FX_RECT(23, 708, 484, 0); break;
//	case 207:rect = FX_RECT(46, 708, 484, 0); break;
//	case 208:rect = FX_RECT(49, 716, 501, 6); break;
//	case 21:rect = FX_RECT(0, 0, 0, 0); break;
//	case 210:rect = FX_RECT(23, 708, 528, 0); break;
//	case 22:rect = FX_RECT(0, 0, 0, 0); break;
//	case 23:rect = FX_RECT(0, 0, 0, 0); break;
//	case 24:rect = FX_RECT(123, 708, 343, 0); break;
//	case 25:rect = FX_RECT(123, 708, 343, 0); break;
//	case 26:rect = FX_RECT(123, 708, 343, 0); break;
//	case 27:rect = FX_RECT(123, 708, 343, 0); break;
//	case 33:rect = FX_RECT(0, 0, 0, 0); break;
//	case 35:rect = FX_RECT(0, 0, 0, 0); break;
//	case 36:rect = FX_RECT(0, 0, 0, 0); break;
//	case 37:rect = FX_RECT(0, 0, 0, 0); break;
//	case 38:rect = FX_RECT(0, 0, 0, 0); break;
//	case 39:rect = FX_RECT(0, 0, 0, 0); break;
//	case 4:rect = FX_RECT(54, 85, 196, 133); break;
//	case 40:rect = FX_RECT(0, 0, 0, 0); break;
//	case 41:rect = FX_RECT(0, 0, 0, 0); break;
//	case 42:rect = FX_RECT(0, 0, 0, 0); break;
//	case 43:rect = FX_RECT(0, 0, 0, 0); break;
//	case 44:rect = FX_RECT(0, 0, 0, 0); break;
//	case 45:rect = FX_RECT(0, 0, 0, 0); break;
//	case 46:rect = FX_RECT(0, 0, 0, 0); break;
//	case 47:rect = FX_RECT(0, 0, 0, 0); break;
//	case 48:rect = FX_RECT(0, 0, 0, 0); break;
//	case 49:rect = FX_RECT(0, 0, 0, 0); break;
//	case 52:rect = FX_RECT(40, 530, 434, 11); break;
//	case 53:rect = FX_RECT(34, 530, 355, 11); break;
//	case 54:rect = FX_RECT(40, 530, 434, 11); break;
//	case 55:rect = FX_RECT(34, 530, 355, 11); break;
//	case 56:rect = FX_RECT(40, 530, 434, 11); break;
//	case 57:rect = FX_RECT(34, 530, 355, 11); break;
//	case 58:rect = FX_RECT(41, 530, 459, 11); break;
//	case 59:rect = FX_RECT(20, 665, 313, 11); break;
//	case 60:rect = FX_RECT(41, 530, 459, 11); break;
//	case 61:rect = FX_RECT(20, 665, 313, 11); break;
//	case 62:rect = FX_RECT(40, 530, 434, 11); break;
//	case 63:rect = FX_RECT(20, 530, 313, 11); break;
//	case 64:rect = FX_RECT(40, 530, 434, 11); break;
//	case 65:rect = FX_RECT(34, 530, 355, 11); break;
//	case 66:rect = FX_RECT(34, 530, 355, 11); break;
//	case 67:rect = FX_RECT(34, 530, 355, 11); break;
//	case 68:rect = FX_RECT(34, 530, 355, 0); break;
//	case 69:rect = FX_RECT(20, 530, 313, 0); break;
//	case 70:rect = FX_RECT(34, 530, 315, 0); break;
//	case 71:rect = FX_RECT(20, 530, 313, 0); break;
//	case 72:rect = FX_RECT(20, 85, 179, 0); break;
//	case 73:rect = FX_RECT(20, 530, 313, 0); break;
//	case 74:rect = FX_RECT(20, 85, 179, 0); break;
//	case 75:rect = FX_RECT(20, 530, 313, 0); break;
//	case 76:rect = FX_RECT(20, 85, 179, 0); break;
//	case 77:rect = FX_RECT(20, 530, 313, 0); break;
//	case 78:rect = FX_RECT(16, 530, 313, 0); break;
//	case 79:rect = FX_RECT(20, 142, 209, 0); break;
//	case 8:rect = FX_RECT(54, 85, 196, 0); break;
//	case 80:rect = FX_RECT(20, 530, 313, 0); break;
//	case 81:rect = FX_RECT(20, 85, 179, 0); break;
//	case 82:rect = FX_RECT(20, 142, 209, 0); break;
//	case 83:rect = FX_RECT(20, 142, 209, 0); break;
//	case 84:rect = FX_RECT(20, 85, 179, 0); break;
//	case 85:rect = FX_RECT(13, 85, 179, 0); break;
//	case 86:rect = FX_RECT(20, 85, 179, 0); break;
//	case 87:rect = FX_RECT(16, 85, 179, 0); break;
//	case 88:rect = FX_RECT(16, 85, 179, 0); break;
//	case 89:rect = FX_RECT(23, 85, 179, 0); break;
//	case 9:rect = FX_RECT(123, 708, 343, 0); break;
//	case 90:rect = FX_RECT(13, 85, 179, 0); break;
//	case 91:rect = FX_RECT(16, 85, 179, 0); break;
//	case 92:rect = FX_RECT(13, 85, 179, 0); break;
//	case 93:rect = FX_RECT(16, 85, 179, 0); break;
//	case 94:rect = FX_RECT(16, 85, 179, 0); break;
//	case 95:rect = FX_RECT(16, 85, 179, 0); break;
//	case 96:rect = FX_RECT(16, 85, 179, 0); break;
//	case 97:rect = FX_RECT(13, 85, 179, 0); break;
//	case 98:rect = FX_RECT(16, 85, 179, 0); break;
//	case 99:rect = FX_RECT(13, 142, 209, 0); break;
//	default:rect = FX_RECT(0, 0, 0, 0);
//	}
//	return true;
//}