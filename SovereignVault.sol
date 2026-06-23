// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;

/// @title SovereignVault
/// @notice Self-custodial deposit/withdraw vault for ETH and ERC-20 tokens.
///         Access control is purely msg.sender-based — no signature scheme
///         is needed because every call is sent directly from the owning
///         EOA (no relayer, no bundler, no account abstraction layer).
contract SovereignVault {
    address public immutable owner;

    bool private locked;

    mapping(address => uint256) private ethBalance;
    mapping(address => mapping(address => uint256)) private tokenBalances; // user => token => amount

    event Deposited(address indexed user, uint256 amount);
    event Withdrawn(address indexed user, uint256 amount);
    event TokenDeposited(address indexed user, address indexed token, uint256 amount);
    event TokenWithdrawn(address indexed user, address indexed token, uint256 amount);

    modifier noReentrant() {
        require(!locked, "Reentrant call");
        locked = true;
        _;
        locked = false;
    }

    constructor() {
        owner = msg.sender;
    }

    // ---------------------------------------------------------------
    // ETH
    // ---------------------------------------------------------------

    /// @notice Deposit ETH into your own vault balance.
    function deposit() external payable {
        require(msg.value > 0, "Zero deposit");
        ethBalance[msg.sender] += msg.value;
        emit Deposited(msg.sender, msg.value);
    }

    /// @notice Allows direct ETH transfers to also count as deposits.
    receive() external payable {
        ethBalance[msg.sender] += msg.value;
        emit Deposited(msg.sender, msg.value);
    }

    /// @notice Withdraw ETH from your own vault balance.
    function withdraw(uint256 amount) external noReentrant {
        require(ethBalance[msg.sender] >= amount, "Insufficient balance");
        ethBalance[msg.sender] -= amount;
        (bool sent, ) = msg.sender.call{value: amount}("");
        require(sent, "Transfer failed");
        emit Withdrawn(msg.sender, amount);
    }

    /// @notice Read your (or anyone's) ETH balance held in the vault.
    function balanceOf(address user) external view returns (uint256) {
        return ethBalance[user];
    }

    // ---------------------------------------------------------------
    // ERC-20
    // ---------------------------------------------------------------

    /// @notice Deposit an ERC-20 token. Requires prior approve() on the token contract.
    function depositToken(address token, uint256 amount) external noReentrant {
        require(amount > 0, "Zero deposit");
        require(IERC20(token).transferFrom(msg.sender, address(this), amount), "Transfer failed");
        tokenBalances[msg.sender][token] += amount;
        emit TokenDeposited(msg.sender, token, amount);
    }

    /// @notice Withdraw a previously deposited ERC-20 token.
    function withdrawToken(address token, uint256 amount) external noReentrant {
        require(tokenBalances[msg.sender][token] >= amount, "Insufficient balance");
        tokenBalances[msg.sender][token] -= amount;
        require(IERC20(token).transfer(msg.sender, amount), "Transfer failed");
        emit TokenWithdrawn(msg.sender, token, amount);
    }

    /// @notice Read your (or anyone's) balance of a specific token held in the vault.
    function tokenBalanceOf(address user, address token) external view returns (uint256) {
        return tokenBalances[user][token];
    }
}

interface IERC20 {
    function transfer(address to, uint256 amount) external returns (bool);
    function transferFrom(address from, address to, uint256 amount) external returns (bool);
    function balanceOf(address account) external view returns (uint256);
}
