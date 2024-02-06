/*
* Copyright 2016 Nu-book Inc.
* Copyright 2016 ZXing authors
*/
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <memory>

#include "ZXing/Content.h"
#include "ZXing/Error.h"

namespace ZXing {

class CustomData;

class DecoderResult
{
	Content _content;
	std::string _ecLevel;
	int _lineCount = 0;
	int _versionNumber = 0;
	Error _error;
	std::shared_ptr<CustomData> _extra;

	DecoderResult(const DecoderResult &) = delete;
	DecoderResult& operator=(const DecoderResult &) = delete;

public:
	const Content& content() const & { return _content; }
	Content&& content() && { return std::move(_content); }

	const Error& error() const & { return _error; }
	Error&& error() && { return std::move(_error); }

};

} // ZXing
