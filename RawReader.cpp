#include "ZXing/ReadBarcode.h"
#include "ZXing/ZXVersion.h"
#include "QRDecoder.h"
#include "ZXing/BitMatrix.h"
#include "ZXing/BitMatrixIO.h"
#include "DecoderResult.h"

#include <cctype>
#include <cstring>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	std::cout.setf(std::ios::boolalpha);

	std::string input;
	int lineSize = 0;
	for (std::string line; std::getline(std::cin, line) && line != "#";) {
		if (lineSize == 0) {
			lineSize = line.size();
		} else if (line.size() != lineSize) {
			while (line.size() < lineSize) {
				line += ' ';
			}
		}
		input += line + '\n';
	}

	const auto bitMatrix = ZXing::ParseBitMatrix(input, 'X', false);

	ZXing::DecoderResult result = ZXing::QRCode::Decode(bitMatrix);

	int ret = static_cast<int>(result.error().type());

	if (ret != 0) {
		return ret;
	}

	std::cout << result.content().text(ZXing::TextMode::HRI);

	return 0;
}
