#pragma once

#include <bits/stdc++.h>

namespace sky {
	std::vector<std::string> split(const std::string& s, const std::string& delim) {
		std::stringstream ss(s);
		std::vector<std::string> output;

		std::string tok;
		while(getline(ss, tok, delim[0])) {
			output.push_back(tok);
		};
		return output;
	}
}
