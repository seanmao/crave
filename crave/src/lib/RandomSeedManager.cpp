// Copyright 2012-2016 The CRAVE developers, University of Bremen, Germany. All rights reserved.//
#include "../crave/RandomSeedManager.hpp"
#include <functional>

RandomSeedManager::RandomSeedManager(unsigned int seed) : default_rng_(seed), seed_(seed) {}

RandomSeedManager::~RandomSeedManager() {
  for(random_map_t::value_type & entry : randomMap_) { delete entry.second; }
}

void RandomSeedManager::set_global_seed(unsigned int s) {
  seed_ = s;
  default_rng_.seed(s);
}

unsigned int RandomSeedManager::get_seed() {
  return seed_;
}

#ifndef WITH_SYSTEMC

std::mt19937* RandomSeedManager::get() { return &default_rng_; }

#else

#include <systemc>
std::mt19937* RandomSeedManager::get() {
	static std::hash<std::string> string_hash;
	auto process = sc_core::sc_get_current_process_handle();
	if (!process.valid())
		return &default_rng_;
	if (randomMap_.count(process.name())) {  // constains
		return randomMap_.at(process.name());
	}
	std::mt19937* rnd = new std::mt19937(
			seed_ + 19937 * string_hash(process.name()));
	randomMap_.insert(std::make_pair(process.name(), rnd));
	return rnd;
}

#endif
