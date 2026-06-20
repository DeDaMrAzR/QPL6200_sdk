// CompactRandom.h : header file
//
// Copyright(C) 2014-2024 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) generic Compact Series framework
//
// This file provides random number utility functions

uint32_t Randomize(const uint32_t lowerBound, const uint32_t range);
uint64_t Randomize2(const uint64_t lowerBound, const uint32_t range);

uint16_t rand16();
uint32_t rand32();
uint64_t rand64();
