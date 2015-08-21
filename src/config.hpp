/*
 * Renzoku - Re-build, re-test, and re-run a program whenever the code changes
 * Copyright (C) 2015  Colton Wolkins
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
/*
 * =====================================================================================
 *
 *       Filename:  config.hpp
 *
 *    Description:  Configuration file retrieval
 *
 *        Version:  1.0
 *        Created:  05/23/2015 10:51:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *
 * =====================================================================================
 */
#include <vector>
#include <string>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

/*
 * Watcher {
 *     Filter = "*.cpp", "*.hpp", "*.c", "*.h", "include/*.hxx";
 *     Include = "Makefile";
 *     Exclude = "src/Config.cpp";
 * }
 * Compiler {
 *     Command = "make";
 *     Enabled = True;
 * }
 * Tester {
 *     Command = "make test";
 *     IgnoreStatus = 2,3,4;
 *     Enabled = True;
 * }
 * Coverage { }; // Future DLL?
 * Runner {
 *     Command = "bin/program --workdir=\"$cwd\"";
 *     Enabled = True;
 * }
 */

class config_exception : public std::runtime_error {
	public:
		config_exception() : std::runtime_error("Bad things") {
		}
		virtual const char* what() const noexcept
		{
			return "Config exception";
		}
};

class iConfigSection {
	public:
		// virtual void registerSection() = 0; // This will be handled by a macro
		virtual void readSection(std::istream&) = 0;
		virtual void writeSection() = 0;
	protected:
		// Temporary
		// TODO: Convert this functionality into a tokenizer/lexer
		bool mHitNewline = true;
		bool isWhitespace(const char &c)
		{
			if(c == '\n' || c == '\r')
				mHitNewline = true;
			else if( c != ' ' && c != '\t' )
				mHitNewline = false;
			return mHitNewline && ( c == ' ' || c == '\t' || c == '\n' || c == '\r');
		}
		std::string getLine(std::istream &file) {
			std::string line;
			int endchar, endschar;
			char c;
			while(file.get(c)){
				if(this->isWhitespace(c))
					continue;
				line += c;
				if(c == ';' || c == '}')
					break;
			}
			if(line.length() == 0)
				return "";
			endchar = line.find(';');
			endschar = line.find('}');
			if(endschar >= 0)
			{
				const char* str = line.c_str();
				c = str[0];
				int i = 0;
				while( c != 0 ) {
					if(this->isWhitespace(c))
					{
						c = str[i++];
					}
					else if(i == endschar)
						return "}";
					else
						throw config_exception();
				}
			}
			return line;
			//if(endchar<0)
			//	throw config_exception();
		}
		std::string* getTokenized(const std::string &str) {
			std::string* ret = new std::string[2];
			size_t len = str.length();
			size_t i = 0;
			short state = 0;
			char c;
			for(i = 0; i < len; i++) {
				c = str[i];
				if(this->isWhitespace(c)) continue;
				switch(state) {
					case 0:
						if(c == '=') {
							state++;
							continue;
						}
						ret[0] +=c;
						break;
					case 1:
						if(c != '=' && !this->isWhitespace(c))
							goto endloop;
				}
			}
endloop:
			while(i < len) {
				ret[1] +=c;
				c = str[++i];
			}
			return ret;
		}
};
/*
typedef enum {
	WS, eof, newline,
	command, lbracket, rbracket,
	name, equals, number, boolean, string, semicolon
} Symbol;
class TLexer {
	private:
		enum ConfigSymbols { };
		std::string data;
		ConfigSymbols getToken();
		std::string getString();
};
class TTokenizer {
	private:
		std::istream& file;
		bool reading = true;
	public:
		TTokenizer(std::istream &file) {
			this->file = file;
		}
		std::string nextToken() {
			std::string line;
			int endchar, endschar;
			char c;
			bool instring = false;
			line = "";
			while(file.get(c)){
				line += c;
				if(c == '"')
					instring = !instring;
				if(file.peek() == ';' && !instring)
					break;
			}
			if(line.length() == 0)
				return "";
			endchar = line.find(';');
			endschar = line.find('}');
			if(endschar>=0)
				reading = false;
			std::cout << line << '\n';
			if(endchar<0)
				throw config_exception();
		}
};// */
class Compiler : public iConfigSection {
	private:
		std::vector<std::string> mCommand;
		bool mEnabled;
	public:
		void readSection(std::istream &file) {
			bool reading = true;
			std::string line;
			int endchar, endschar;
			char c;
			while(reading){
				line = "";
				line = this->getLine(file);
				std::cout << line << '\n';
				if(line == "}")
					reading = false;
			}
		}
		void writeSection() {
		}
};
class Tester : public iConfigSection {
		std::vector<std::string> mCommand;
		std::vector<short> mIgnoredReturnCodes;
		bool mEnabled;
	public:
		void readSection(std::istream &file) {
		}
		void writeSection() {
		}
};
class Runner : public iConfigSection {
	private:
		std::vector<std::string> mCommand;
		bool mEnabled;
	public:
		void readSection(std::istream &file) {
		}
		void writeSection() {
		}
};
/*
class Watcher : public iConfigSection {
	private:
		std::vector<std::string> mFilter;
		std::vector<std::string> mIncludeList;
		std::vector<std::string> mExcludeList;
}
class Compiler : public iConfigSection {
	private:
		std::vector<std::string> mCommand;
		bool mEnabled;
}
class Tester : public iConfigSection {
		std::vector<std::string> mCommand;
		std::vector<short> mIgnoredReturnCodes;
		bool mEnabled;
}
class Runner : public iConfigSection {
	private:
		std::vector<std::string> mCommand;
		bool mEnabled;
}

// */

class Config {
	/* Config file format:
	 * project XXXXXX
	 * \tbuildcmd XXXX XXXX
	 * \truncmd XXXX
	 * #vim: set tw=8 sw=8 ai noet:
	 */
	private:
		Compiler compiler;
		void readConfig(std::istream &is)
		{
			std::string line;
			//std::string prefix = "Compiler";
			std::string prefix = "Watcher";
			while(is.good()) {
				std::getline(is, line);
				if(line.substr(0, prefix.size()) == prefix) {
					compiler.readSection(is);
					break;
				}
			}
		}
	public:
		std::vector<std::string> sections = {
			"test",
#define CONFIG_SECTION(X) #X,
#include "config_sections.hpp"
#undef CONFIG_SECTION
		};
		Config(std::stringstream &is)
		{
			readConfig(is);
		}
		std::vector<std::string> getProjects();
		std::string getBuildCommand(std::string project);
		std::string getTestCommand(std::string project);
		std::string getRunCommand(std::string project);
};
