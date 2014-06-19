/* libafc - utils to facilitate C++ development.
Copyright (C) 2014 Dźmitry Laŭčuk

libafc is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef ENSURE_ASCII_HPP_
#define ENSURE_ASCII_HPP_

// Verifying non-printable characters that have escape sequences.
static_assert(u8"\0"[0] == '\0' && u8"\a"[0] == '\a' && u8"\b"[0] == '\b' && u8"\t"[0] == '\t' &&
				u8"\n"[0] == '\n' && u8"\v"[0] == '\v' && u8"\f"[0] == '\f' && u8"\r"[0] == '\r' &&
				u8"\e"[0] == '\e',
		"Non-ASCII default charset is detected. It is not supported.");

// Verifying printable characters.
static_assert(u8" "[0] == ' ' && u8"!"[0] == '!' && u8"\""[0] == '"' && u8"#"[0] == '#' &&
				u8"$"[0] == '$' && u8"%"[0] == '%' && u8"&"[0] == '&' && u8"'"[0] == '\'' &&
				u8"("[0] == '(' && u8")"[0] == ')' && u8"*"[0] == '*' && u8"+"[0] == '+' &&
				u8","[0] == ',' && u8"-"[0] == '-' && u8"."[0] == '.' && u8"/"[0] == '/' &&
				u8"0"[0] == '0' && u8"1"[0] == '1' && u8"2"[0] == '2' && u8"3"[0] == '3' &&
				u8"4"[0] == '4' && u8"5"[0] == '5' && u8"6"[0] == '6' && u8"7"[0] == '7' &&
				u8"8"[0] == '8' && u8"9"[0] == '9' && u8":"[0] == ':' && u8";"[0] == ';' &&
				u8"<"[0] == '<' && u8"="[0] == '=' && u8">"[0] == '>' && u8"?"[0] == '?' &&
				u8"@"[0] == '@' && u8"A"[0] == 'A' && u8"B"[0] == 'B' && u8"C"[0] == 'C' &&
				u8"D"[0] == 'D' && u8"E"[0] == 'E' && u8"F"[0] == 'F' && u8"G"[0] == 'G' &&
				u8"H"[0] == 'H' && u8"I"[0] == 'I' && u8"J"[0] == 'J' && u8"K"[0] == 'K' &&
				u8"L"[0] == 'L' && u8"M"[0] == 'M' && u8"N"[0] == 'N' && u8"O"[0] == 'O' &&
				u8"P"[0] == 'P' && u8"Q"[0] == 'Q' && u8"R"[0] == 'R' && u8"S"[0] == 'S' &&
				u8"T"[0] == 'T' && u8"U"[0] == 'U' && u8"V"[0] == 'V' && u8"W"[0] == 'W' &&
				u8"X"[0] == 'X' && u8"Y"[0] == 'Y' && u8"Z"[0] == 'Z' && u8"["[0] == '[' &&
				u8"\\"[0] == '\\' && u8"]"[0] == ']' && u8"^"[0] == '^' && u8"_"[0] == '_' &&
				u8"`"[0] == '`' && u8"a"[0] == 'a' && u8"b"[0] == 'b' && u8"c"[0] == 'c' &&
				u8"d"[0] == 'd' && u8"e"[0] == 'e' && u8"f"[0] == 'f' && u8"g"[0] == 'g' &&
				u8"h"[0] == 'h' && u8"i"[0] == 'i' && u8"j"[0] == 'j' && u8"k"[0] == 'k' &&
				u8"l"[0] == 'l' && u8"m"[0] == 'm' && u8"n"[0] == 'n' && u8"o"[0] == 'o' &&
				u8"p"[0] == 'p' && u8"q"[0] == 'q' && u8"r"[0] == 'r' && u8"s"[0] == 's' &&
				u8"t"[0] == 't' && u8"u"[0] == 'u' && u8"v"[0] == 'v' && u8"w"[0] == 'w' &&
				u8"x"[0] == 'x' && u8"y"[0] == 'y' && u8"z"[0] == 'z' && u8"{"[0] == '{' &&
				u8"|"[0] == '|' && u8"}"[0] == '}' && u8"~"[0] == '~',
		"Non-ASCII default charset is detected. It is not supported.");

#endif /* ENSURE_ASCII_HPP_ */
