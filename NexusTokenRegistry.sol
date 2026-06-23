// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;

contract NexusTokenRegistry {
    address public rootAdmin;
    address public treasuryAllocation;

    constructor(address _rootAdmin, address _treasuryAllocation) {
        rootAdmin = _rootAdmin;
        treasuryAllocation = _treasuryAllocation;
    }
}
