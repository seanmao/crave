// Copyright 2015-2016 The CRAVE developers, University of Bremen, Germany. All rights reserved.//

#pragma once

#include <crave/experimental/SystemC.hpp>
#include <crave/ConstrainedRandom.hpp>
#include <crave/frontend/Constraint.hpp>
#include <crave/experimental/Experimental.hpp>

#include <uvm>
/**
 * \brief Overrides uvm_sequence and crv_sequence_item to merge its functionality.
 *
 * This class is intendant as an adapter to merge CRAVE functionality into UVM-SystemC
 * processes without harming them. To use it, simply inherit from this class instead from
 * uvm_sequence. You may now use CRAVE variables and constraints in your UVM-Sequence.
 */

template<typename REQ = uvm::uvm_sequence_item, typename RSP = REQ>
class uvm_randomized_sequence : public uvm::uvm_sequence<REQ, RSP>, public crave::crv_sequence_item {
public:
    /**
     * \brief Register uvm_randomized_sequence with UVM-SystemC.
     */
    UVM_OBJECT_UTILS(uvm_randomized_sequence);

    /**
     * \brief Creates a new sequence with given name.
     *
     * Since both CRAVE and UVM-SystemC are using some kind of name for theier objects
     * , both parent will be named the same. std::string is the lowest common denominator
     * between the nameing type of CRAVE and of UVM-SystemC. Therefore you may pass a
     * std::string or a crave::crv_object_name as argument.
     *
     * \param name Name of this sequence.
     */
    uvm_randomized_sequence(crave::crv_object_name name)
    : uvm::uvm_sequence<REQ, RSP>(name()) {
    };

    virtual ~uvm_randomized_sequence() {
    };
};

/**
 * \example same_flavored_jelly_beans_sequence.h
 *
 * Demonstrates how to inherit from this class and use CRAVE to ensure that a special
 * attribute remains the same wihthin this sequence.
 */
/**
 * \example gift_boxed_jelly_beans_sequence.h
 *
 * Shows how to randomize the amount of sequences within a sequence with CRAVE.
 */
