#include <iostream>
#include <vector>
#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

const int IntroDelay = 90;
const int PulseDelay = 120;
HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

void TextColor(int color) {
	SetConsoleTextAttribute(ConsoleHandle, color);
}

void gotoXY(int x, int y) {
	COORD coord;
	coord.X = static_cast<SHORT>(x);
	coord.Y = static_cast<SHORT>(y);
	SetConsoleCursorPosition(ConsoleHandle, coord);
}

void setCursorVisible(bool visible) {
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(ConsoleHandle, &cursor_info);
	cursor_info.bVisible = visible;
	SetConsoleCursorInfo(ConsoleHandle, &cursor_info);
}

vector<string> input(const char *path) {
	ifstream input_file(path);
	vector<string> lines;
	string line;
	while (getline(input_file, line)) {
		lines.push_back(line);
	}
	return lines;
}

bool hasMinLength(const vector<string> &lines, int from, int to, size_t min_length) {
	if (from < 0 || to >= static_cast<int>(lines.size()) || from > to)
		return false;
	for (int i = from; i <= to; i++) {
		if (lines[i].size() < min_length)
			return false;
	}
	return true;
}

bool validateInputs(const vector<string> &may_tinh, const vector<string> &s0, const vector<string> &s1, const vector<string> &s2) {
	if (may_tinh.size() < 9 || s0.empty())
		return false;
	if (!hasMinLength(may_tinh, 0, static_cast<int>(may_tinh.size()) - 1, 43))
		return false;
	if (may_tinh[8].size() < 46)
		return false;
	if (s1.size() < 21 || s2.size() < 21)
		return false;
	if (!hasMinLength(s1, 5, 20, 61) || !hasMinLength(s2, 5, 20, 61))
		return false;
	return true;
}

char renderHeartChar(char ch) {
	if (ch == '@' || ch == '3')
		return char(3);
	return ch;
}

int heartColor(char ch, bool pulse) {
	if (ch == '@')
		return pulse ? 13 : 12;
	if (ch == '3')
		return pulse ? 15 : 12;
	if (ch == '*')
		return 15;
	if (ch == '|' || ch == '!')
		return 14;
	if (ch == '@')
		return 12;
	if (ch == ':' )
		return 8;
	if (ch == '.' || ch == '\'' || ch == '`' || ch == ',')
		return 7;
	return 7;
}

void drawStringAt(int x, int y, const string &text, int color) {
	TextColor(color);
	gotoXY(x, y);
	cout << text;
}

void drawHeartFrame(int x, int y, const vector<string> &frame, bool pulse) {
	for (int row = 0; row < static_cast<int>(frame.size()); row++) {
		for (int col = 0; col < static_cast<int>(frame[row].size()); col++) {
			TextColor(heartColor(frame[row][col], pulse));
			gotoXY(x + col, y + row);
			cout << renderHeartChar(frame[row][col]);
		}
	}
}

void drawHeartReveal(int x, int y, const vector<string> &frame) {
	for (int row = 20; row >= 5; row--) {
		for (int col = 17; col < 61; col++) {
			TextColor(heartColor(frame[row][col], false));
			gotoXY(x + col, y + row);
			cout << renderHeartChar(frame[row][col]);
		}
		Sleep(IntroDelay);
	}
}

void drawHeartFill(int x, int y, const vector<string> &frame, int color) {
	for (int row = 20; row >= 5; row--) {
		for (int col = 0; col < 61; col++) {
			if (frame[row][col] != '@' && frame[row][col] != '3')
				continue;
			if (col > 30 && col < 47 && row <= 8)
				continue;
			TextColor(color);
			gotoXY(x + col, y + row);
			cout << char(3);
		}
		Sleep(IntroDelay);
	}

	TextColor(color);
	drawStringAt(x + 22, y + 4, string(11, char(3)), color);
	drawStringAt(x + 44, y + 4, string(11, char(3)), color);
	Sleep(IntroDelay);
	drawStringAt(x + 31, y + 5, string(5, char(3)), color);
	drawStringAt(x + 41, y + 5, string(5, char(3)), color);
	Sleep(IntroDelay);
	drawStringAt(x + 35, y + 6, string(3, char(3)), color);
	drawStringAt(x + 39, y + 6, string(3, char(3)), color);
	Sleep(IntroDelay);
	drawStringAt(x + 38, y + 7, string(1, char(3)), color);
}

void drawPulse(int x, int y, const vector<string> &frame, int color) {
	for (int row = 4; row <= 20; row++) {
		for (int col = 0; col < 61; col++) {
			if (frame[row][col] != '@' && frame[row][col] != '3')
				continue;
			TextColor(color);
			gotoXY(x + col, y + row);
			cout << char(3);
		}
	}
	TextColor(color);
	drawStringAt(x + 22, y + 4, string(11, char(3)), color);
	drawStringAt(x + 44, y + 4, string(11, char(3)), color);
	drawStringAt(x + 31, y + 5, string(5, char(3)), color);
	drawStringAt(x + 41, y + 5, string(5, char(3)), color);
	drawStringAt(x + 35, y + 6, string(3, char(3)), color);
	drawStringAt(x + 39, y + 6, string(3, char(3)), color);
	drawStringAt(x + 38, y + 7, string(1, char(3)), color);
}

void drawMonitorExpression(int x, int y, const string &eyes, const string &mouth, int color) {
	drawStringAt(x + 12, y + 3, eyes, color);
	drawStringAt(x + 12, y + 5, mouth, color);
}

void MayTinh(int x, int y, const vector<string> &may_tinh) {
	for (int i = 0; i < static_cast<int>(may_tinh.size()); i++) {
		string left_part = may_tinh[i].substr(0, i == 8 ? 20 : 25);
		string right_part = i == 8 ? "  " + may_tinh[i].substr(45) : may_tinh[i].substr(43);
		drawStringAt(x, y + i, left_part, 7);
		drawStringAt(x + 44, y + i, right_part, 7);
	}

	for (int i = 0; i <= 12; i++) {
		TextColor(11);
		gotoXY(x + i + 21, y + 8);
		cout << may_tinh[8][i + 21];
		gotoXY(x + 45 - i, y + 8);
		cout << may_tinh[8][45 - i];
		Sleep(IntroDelay);
	}

	for (int i = 0; i < 8; i++) {
		TextColor(12);
		gotoXY(x + 32 - i, y + 8 - i);
		cout << may_tinh[8 - i].substr(32 - i, i * 2 + 3);
		Sleep(IntroDelay);
	}

	drawMonitorExpression(x, y, "v   v", "\\___/", 15);
}

void heart(int x, int y, int monitor_x, int monitor_y, const vector<string> &s0, const vector<string> &s1, const vector<string> &s2) {
	while (1) {
		drawMonitorExpression(monitor_x, monitor_y, "-   -", "\\___/", 11);
		drawHeartFrame(x, y, s0, false);
		Sleep(IntroDelay);

		drawMonitorExpression(monitor_x, monitor_y, "o   o", "\\___/", 14);
		drawHeartReveal(x, y, s1);
		Sleep(IntroDelay * 2);

		drawMonitorExpression(monitor_x, monitor_y, "O   O", "\\_U_/", 12);
		drawHeartFill(x, y, s2, 12);
		Sleep(PulseDelay);

		for (int pulse = 0; pulse < 3; pulse++) {
			drawPulse(x, y, s2, pulse % 2 == 0 ? 13 : 12);
			drawMonitorExpression(monitor_x, monitor_y, pulse % 2 == 0 ? "^   ^" : "o   o", "\\___/", 15);
			Sleep(PulseDelay);
		}

		drawMonitorExpression(monitor_x, monitor_y, "v   v", "\\___/", 15);
		Sleep(PulseDelay * 4);
	}
}

int main() {
	system("cls");
	setCursorVisible(false);
	SetConsoleTitleA("Trai tim ASCII");

	int x = 20, y = 2;
	int monitor_x = x + 5, monitor_y = y + 30;
	const char *may_tinh_input = "./may tinh.txt";
	const char *heart_0 = "./heart0.txt";
	const char *heart_1 = "./heart1.txt";
	const char *heart_2 = "./heart2.txt";

	vector<string> may_tinh = input(may_tinh_input);
	vector<string> s0 = input(heart_0);
	vector<string> s1 = input(heart_1);
	vector<string> s2 = input(heart_2);

	if (!validateInputs(may_tinh, s0, s1, s2)) {
		cerr << "Khong the doc du lieu hop le tu cac file txt." << endl;
		return 1;
	}

	MayTinh(monitor_x, monitor_y, may_tinh);
	heart(x, y, monitor_x, monitor_y, s0, s1, s2);
	return 0;
}
