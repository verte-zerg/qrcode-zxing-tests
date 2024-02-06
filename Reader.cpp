/*
* Copyright 2016 Nu-book Inc.
* Copyright 2019 Axel Waggershauser
*/
// SPDX-License-Identifier: Apache-2.0

#include "ZXing/ReadBarcode.h"
#include "ZXing/ZXVersion.h"

#include <cctype>
#include <chrono>
#include <cstring>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace ZXing;

int main(int argc, char* argv[])
{
	ReaderOptions options;
	std::vector<std::string> filePaths;
	Results allResults;
	int ret = 0;

	options.setTextMode(TextMode::HRI);
	options.setEanAddOnSymbol(EanAddOnSymbol::Read);

	options.setTryHarder(false);
	options.setTryRotate(false);
	options.setTryInvert(false);
	options.setTryDownscale(false);
	options.setFormats(BarcodeFormatsFromString("QRCode|MicroQRCode"));
	options.setIsPure(true);
	for (int i = 1; i < argc; ++i) {
		filePaths.push_back(argv[i]);
	}

	if (filePaths.empty()) {
		std::cout << "Usage: " << argv[0] << " <image file>...\n";
		return -1;
	}

	std::cout.setf(std::ios::boolalpha);

	for (const auto& filePath : filePaths) {
		int width, height, channels;
		std::unique_ptr<stbi_uc, void(*)(void*)> buffer(stbi_load(filePath.c_str(), &width, &height, &channels, 3), stbi_image_free);
		if (buffer == nullptr) {
			std::cerr << "Failed to read image: " << filePath << " (" << stbi_failure_reason() << ")" << "\n";
			return -1;
		}

		ImageView image{buffer.get(), width, height, ImageFormat::RGB};
		auto results = ReadBarcodes(image, options);

		if (results.empty())
			results.emplace_back();

		allResults.insert(allResults.end(), results.begin(), results.end());
		if (filePath == filePaths.back()) {
			auto merged = MergeStructuredAppendSequences(allResults);
			// report all merged sequences as part of the last file to make the logic not overly complicated here
			results.insert(results.end(), std::make_move_iterator(merged.begin()), std::make_move_iterator(merged.end()));
		}

		for (auto&& result : results) {
			ret |= static_cast<int>(result.error().type());

			if (filePaths.size() > 1 || results.size() > 1) {
				static bool firstFile = true;
				if (!firstFile)
					std::cout << "\n";
				if (filePaths.size() > 1)
					std::cout << "File:       " << filePath << "\n";
				firstFile = false;
			}

			if (result.format() == BarcodeFormat::None) {
				std::cout << "No barcode found\n";
				continue;
			}

			std::cout << result.text();
		}
	}

	return ret;
}
