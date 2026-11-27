# QVAULT v2.1: what was built and what is left

As of November 26, 2026 · PJDUBSss

All six v2.1 changes are written in this fork and pass all 17 QVAULT and QX tests. Five steps remain before the code goes to the Qubic core team.

## At a glance

Each pull request builds on the one before, so [#4](https://github.com/PJDUBSss/core/pull/4) holds all of v2.1 and is the branch to hand over.

| Change | What it does | Tests | PR |
| --- | --- | --- | --- |
| Frozen stake escrow | Moves QCAP staked by frozen (breached) wallets to the core team's FROZEN_ESCROW | Pass | [#1](https://github.com/PJDUBSss/core/pull/1) |
| No 1-week delay | Passed fundraising, quorum and allocation proposals take effect when the next epoch begins | Pass | [#1](https://github.com/PJDUBSss/core/pull/1) |
| 1. Release of funds | Treasury proposal releases Qu from the reinvesting fund or unsold QCAP to a named wallet | Pass | [#2](https://github.com/PJDUBSss/core/pull/2) |
| 2. Token handling | Treasury proposal sends tokens QVAULT holds to a wallet, or sells them on QX | Pass | [#2](https://github.com/PJDUBSss/core/pull/2) |
| 4. Ecosystem poll voting | Stakers vote in QVAULT; QVAULT casts all its shares in the contract's poll for the leading option | Pass | [#3](https://github.com/PJDUBSss/core/pull/3) |
| 3. Lock in wallet | Staked QCAP stays in the staker's wallet, locked, instead of moving into QVAULT | Pass | [#4](https://github.com/PJDUBSss/core/pull/4) |

## How each change works

### Frozen stake escrow

The core froze 2,806 weak seed addresses and moved their Qu and wallet held shares to FROZEN_ESCROW, but QCAP staked in QVAULT was missed. At the start of the upgrade epoch, QVAULT sends each frozen staker's QCAP to FROZEN_ESCROW, removes them from the staker and voting power lists, and logs the wallet and amount. Owners then recover it with their other frozen assets.

### No 1-week delay

Votes are still counted when the voting epoch ends. For proposals submitted from the upgrade epoch on, quorum changes and allocations apply when the next epoch begins, and a passed QCAP sale opens in that epoch. Older proposals keep the old timing so nothing scheduled is skipped. QEarn timing is unchanged.

### Treasury proposal (items 1 and 2)

A new proposal type with three actions, carried out at the end of the epoch it passes in, oldest first:

- **Send Qu:** from the reinvesting fund only, to a wallet named in the proposal.
- **Send tokens:** QX-managed tokens QVAULT owns, including unsold QCAP (counted toward the yearly sale limits). Staked QCAP can never be released.
- **Sell tokens:** a QX sell order at the price in the proposal. Qu from sales counts as revenue and is distributed. QCAP sales stay with fundraising proposals.

A proposal that passes but cannot be carried out moves nothing and is marked as failed.

### Ecosystem poll voting (item 4)

A bot creates a QVAULT poll proposal when a shareholder poll opens in a contract where QVAULT holds shares. Stakers pick an option and can change it while the poll is open. After every staker vote, QVAULT casts all its shares in that poll for the leading option; a tie keeps the current option.

### Lock in wallet (item 3)

Staking records a lock on QCAP in the staker's own wallet instead of moving it into QVAULT. Locked QCAP cannot be released to QX or taken by another contract; unlocked QCAP in the same wallet moves freely. Unstaking only removes the lock. Stakers are paid revenue through their wallet holdings like other holders, so the old separate staker payout is removed to avoid paying twice.

## At the upgrade epoch

Everything runs once, automatically, in this order. The epoch is a placeholder (243) in all constants until the release epoch is known.

1. The node loads QVAULT's saved state and pads it for the new Treasury and poll storage (about 3 MB in total); existing data is untouched.
2. Start of epoch: every existing stake moves from QVAULT back to its owner's wallet, still locked.
3. Start of epoch: frozen wallets' stakes move from those wallets to FROZEN_ESCROW, and leave the staker and voting power lists.
4. From then on: staking locks QCAP in the wallet, passed proposals take effect the next epoch, and Treasury and poll proposals are open.
5. End of each epoch: every lock is capped at what the wallet still holds, then voting power is taken from the locks.

## Design decisions and limits

- **No direct MSVault or QSwap calls.** A contract can only call contracts launched before it. QVAULT is #10, MSVault #11, QSwap #13, so releases go to a wallet named in the proposal and token sales use QX (#1).
- **Poll votes are cast live.** End of epoch processing runs from the newest contract down, so polls in newer contracts close before QVAULT's end of epoch step. Casting there would always be too late.
- **Winner takes all.** QVAULT casts all its shares for the leading option, matching "QVAULT result feeds the poll".
- **Poll proposals need a bot.** QVAULT cannot read polls of newer contracts, so a bot creates each poll proposal. Same rules as other proposals: 10,000 voting power or a QVAULT share, plus the 10M Qu fee, one per poll.
- **A poll proposal is tied to one poll version.** If the poll's creator edits the poll, QVAULT's vote is refused and a new poll proposal is needed.
- **Qu released only from the reinvesting fund.** Revenue owed to QCAP holders and QVAULT shareholders cannot be released.
- **Locks cannot be bypassed.** Locked QCAP can only leave QVAULT management through QVAULT itself, which refuses it, and QVAULT lets no other contract take QCAP management.

## Testing

All 17 QVAULT and QX tests pass on the full v2.1 branch ([run](https://github.com/PJDUBSss/core/actions/runs/45915570745)). Qubic's own BuildTests workflow only compiles tests, so this fork adds a RunContractTests workflow that actually runs them; it lives only on the fork's main branch and is not part of the change.

| Test | What it proves |
| --- | --- |
| stakedQcapLockedInWallet | Stakes move back to wallets at the switch; locked QCAP cannot be moved or locked twice; revenue is paid once (6,000 QCAP earns exactly twice 3,000); unstaking frees it |
| pollProposalCastsSharesLive | QVAULT's 100 shares in a test contract's poll follow the leading option live, flipping yes, no, yes; bad inputs refused; poll closes after its epoch |
| treasuryProposals | Qu release, oversized release fails, token and unsold QCAP sends (staked QCAP untouched), QX sell order, rejection, no quorum, not enough tokens, fund accounting |
| passedProposalsTakeEffectNextEpoch | Old proposals keep their timing; new quorum, allocation and sale proposals apply the next epoch |
| frozenStakeMovedToEscrowOnce | Frozen stake lands in FROZEN_ESCROW, normal staker untouched; the hardcoded escrow ID matches the core's |
| testingAllProceduresAndFunctions | QVAULT's original test, unchanged, still passes |
| ContractQx (11 tests) | QX still passes, including its own frozen wallet cleanup test |

## Before submitting to qubic/core

- [ ] Set the release epoch in QVAULT_FROZEN_STAKE_ESCROW_EPOCH, QVAULT_FAST_PROPOSAL_EFFECT_EPOCH, QVAULT_LOCK_IN_WALLET_EPOCH and the QVAULT padding entry in contract_def.h (all 243 now)
- [ ] Confirm with the core team how owners claim from FROZEN_ESCROW, and that they accept the QVAULT state padding
- [ ] Build or arrange the poll bot: a wallet with 10,000 voting power that pays the 10M Qu fee per poll
- [ ] Hold a QCAP holder vote on v2.1
- [ ] Ask the core team which branch to target (main or develop), then open one PR from qvault-lock-in-wallet

Code: branch [qvault-lock-in-wallet](https://github.com/PJDUBSss/core/tree/qvault-lock-in-wallet), contract `src/contracts/QVAULT.h`, tests `test/contract_qvault.cpp`.
