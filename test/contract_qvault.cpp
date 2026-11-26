#define NO_UEFI

#include <map>
#include <random>

#include "contract_testing.h"

static std::mt19937_64 rand64;
static constexpr uint64 QVAULT_QCAP_MAX_HOLDERS = 131072;
static constexpr uint64 QVAULT_ISSUE_ASSET_FEE = 1000000000ull;
static constexpr uint64 QVAULT_TOKEN_TRANSFER_FEE = 100;
static constexpr uint32 QVAULT_SMALL_AMOUNT_QCAP_TRANSFER = 1000;
static constexpr uint32 QVAULT_BIG_AMOUNT_QCAP_TRANSFER = 1000000;
static constexpr uint32 QVAULT_QCAP_SOLD_AMOUNT = 3230507;
static constexpr uint64 QVAULT_MAX_REVENUE = 1000000000000ull;
static constexpr uint64 QVAULT_MIN_REVENUE = 100000000000ull;;
static const id QVAULT_CONTRACT_ID(QVAULT_CONTRACT_INDEX, 0, 0, 0);
const id QVAULT_QCAP_ISSUER = ID(_Q, _C, _A, _P, _W, _M, _Y, _R, _S, _H, _L, _B, _J, _H, _S, _T, _T, _Z, _Q, _V, _C, _I, _B, _A, _R, _V, _O, _A, _S, _K, _D, _E, _N, _A, _S, _A, _K, _N, _O, _B, _R, _G, _P, _F, _W, _W, _K, _R, _C, _U, _V, _U, _A, _X, _Y, _E);

static unsigned long long random(unsigned long long minValue, unsigned long long maxValue)
{
    if(minValue > maxValue) 
    {
        return 0;
    }
    return minValue + rand64() % (maxValue - minValue);
}

static id getUser(unsigned long long i)
{
    return id(i, i / 2 + 4, i + 10, i * 3 + 8);
}

static std::vector<id> getRandomUsers(unsigned int totalUsers, unsigned int maxNum)
{
    unsigned long long userCount = random(0, maxNum);
    std::vector<id> users;
    users.reserve(userCount);
    for (unsigned int i = 0; i < userCount; ++i)
    {
        unsigned long long userIdx = random(0, totalUsers - 1);
        users.push_back(getUser(userIdx));
    }
    return users;
}

class QVAULTChecker : public QVAULT, public QVAULT::StateData
{
public:
    void submitGPChecker(uint32 index, id proposer)
    {
        EXPECT_EQ(GP.get(index).currentQuorumPercent, 670);
        EXPECT_EQ(GP.get(index).currentTotalVotingPower, 10000);
        EXPECT_EQ(GP.get(index).numberOfNo, 0);
        EXPECT_EQ(GP.get(index).numberOfYes, 0);
        EXPECT_EQ(GP.get(index).proposedEpoch, 139);
        EXPECT_EQ(GP.get(index).proposer, proposer);
        EXPECT_EQ(GP.get(index).result, 5);
    }

    void submitQCPChecker(uint32 index, id proposer, uint32 newQuorumPercent)
    {
        EXPECT_EQ(QCP.get(index).currentQuorumPercent, 670);
        EXPECT_EQ(QCP.get(index).currentTotalVotingPower, 10000);
        EXPECT_EQ(QCP.get(index).numberOfNo, 0);
        EXPECT_EQ(QCP.get(index).numberOfYes, 0);
        EXPECT_EQ(QCP.get(index).proposedEpoch, 139);
        EXPECT_EQ(QCP.get(index).proposer, proposer);
        EXPECT_EQ(QCP.get(index).result, 5);
        EXPECT_EQ(QCP.get(index).newQuorumPercent, newQuorumPercent);
    }

    void submitIPOPChecker(uint32 index, id proposer, uint32 ipoContractIndex)
    {
        EXPECT_EQ(IPOP.get(index).currentQuorumPercent, 670);
        EXPECT_EQ(IPOP.get(index).currentTotalVotingPower, 10000);
        EXPECT_EQ(IPOP.get(index).numberOfNo, 0);
        EXPECT_EQ(IPOP.get(index).numberOfYes, 0);
        EXPECT_EQ(IPOP.get(index).proposedEpoch, 139);
        EXPECT_EQ(IPOP.get(index).proposer, proposer);
        EXPECT_EQ(IPOP.get(index).result, 4);
        EXPECT_EQ(IPOP.get(index).ipoContractIndex, ipoContractIndex);
        EXPECT_EQ(IPOP.get(index).totalWeight, 0);
        EXPECT_EQ(IPOP.get(index).assignedFund, 0);
    }

    void submitQEarnPChecker(uint32 index, id proposer, uint64 amountPerEpoch, uint32 numberOfEpoch)
    {
        EXPECT_EQ(QEarnP.get(index).currentQuorumPercent, 670);
        EXPECT_EQ(QEarnP.get(index).currentTotalVotingPower, 10000);
        EXPECT_EQ(QEarnP.get(index).numberOfNo, 0);
        EXPECT_EQ(QEarnP.get(index).numberOfYes, 0);
        EXPECT_EQ(QEarnP.get(index).proposedEpoch, 139);
        EXPECT_EQ(QEarnP.get(index).proposer, proposer);
        EXPECT_EQ(QEarnP.get(index).result, 4);
        EXPECT_EQ(QEarnP.get(index).amountOfInvestPerEpoch, amountPerEpoch);
        EXPECT_EQ(QEarnP.get(index).assignedFundPerEpoch, amountPerEpoch);
        EXPECT_EQ(QEarnP.get(index).numberOfEpoch, numberOfEpoch);
    }

    void submitFundPChecker(uint32 index, id proposer, uint32 amountOfQcap, uint64 pricePerOneQcap)
    {
        EXPECT_EQ(FundP.get(index).currentQuorumPercent, 670);
        EXPECT_EQ(FundP.get(index).currentTotalVotingPower, 10000);
        EXPECT_EQ(FundP.get(index).numberOfNo, 0);
        EXPECT_EQ(FundP.get(index).numberOfYes, 0);
        EXPECT_EQ(FundP.get(index).proposedEpoch, 139);
        EXPECT_EQ(FundP.get(index).proposer, proposer);
        EXPECT_EQ(FundP.get(index).result, 5);
        EXPECT_EQ(FundP.get(index).amountOfQcap, amountOfQcap);
        EXPECT_EQ(FundP.get(index).pricePerOneQcap, pricePerOneQcap);
        EXPECT_EQ(FundP.get(index).restSaleAmount, amountOfQcap);   
    }

    void submitMKTPChecker(uint32 index, id proposer, uint64 amountOfQcap, uint64 amountOfQubic, uint64 shareName, uint32 indexOfShare, uint32 amountOfShare)
    {
        EXPECT_EQ(MKTP.get(index).currentQuorumPercent, 670);
        EXPECT_EQ(MKTP.get(index).currentTotalVotingPower, 10000);
        EXPECT_EQ(MKTP.get(index).numberOfNo, 0);
        EXPECT_EQ(MKTP.get(index).numberOfYes, 0);
        EXPECT_EQ(MKTP.get(index).proposedEpoch, 139);
        EXPECT_EQ(MKTP.get(index).proposer, proposer);
        EXPECT_EQ(MKTP.get(index).result, 4);
        EXPECT_EQ(MKTP.get(index).amountOfQcap, amountOfQcap);
        EXPECT_EQ(MKTP.get(index).amountOfQubic, amountOfQubic);
        EXPECT_EQ(MKTP.get(index).shareName, shareName);
        EXPECT_EQ(MKTP.get(index).shareIndex, indexOfShare);
        EXPECT_EQ(MKTP.get(index).amountOfShare, amountOfShare);
    }

    void submitAlloPChecker(uint32 index, id proposer, uint32 reinvested, uint32 burn, uint32 distribute)
    {
        EXPECT_EQ(AlloP.get(index).currentQuorumPercent, 670);
        EXPECT_EQ(AlloP.get(index).currentTotalVotingPower, 10000);
        EXPECT_EQ(AlloP.get(index).numberOfNo, 0);
        EXPECT_EQ(AlloP.get(index).numberOfYes, 0);
        EXPECT_EQ(AlloP.get(index).proposedEpoch, 139);
        EXPECT_EQ(AlloP.get(index).proposer, proposer);
        EXPECT_EQ(AlloP.get(index).result, 5);
        EXPECT_EQ(AlloP.get(index).reinvested, reinvested);
        EXPECT_EQ(AlloP.get(index).burnQcap, burn);
        EXPECT_EQ(AlloP.get(index).distributed, distribute);
    }

    void voteInProposalChecker(uint32 proposalId, uint32 proposalType, uint32 numberOfYes, uint32 numberOfNo)
    {
        switch (proposalType)
        {
        case 1:
            EXPECT_EQ(GP.get(proposalId).numberOfYes, numberOfYes);
            EXPECT_EQ(GP.get(proposalId).numberOfNo, numberOfNo);
            break;
        case 2:
            EXPECT_EQ(QCP.get(proposalId).numberOfYes, numberOfYes);
            EXPECT_EQ(QCP.get(proposalId).numberOfNo, numberOfNo);
            break;
        case 3:
            EXPECT_EQ(IPOP.get(proposalId).numberOfYes, numberOfYes);
            EXPECT_EQ(IPOP.get(proposalId).numberOfNo, numberOfNo);
            break;
        case 4:
            EXPECT_EQ(QEarnP.get(proposalId).numberOfYes, numberOfYes);
            EXPECT_EQ(QEarnP.get(proposalId).numberOfNo, numberOfNo);
            break;
        case 5:
            EXPECT_EQ(FundP.get(proposalId).numberOfYes, numberOfYes);
            EXPECT_EQ(FundP.get(proposalId).numberOfNo, numberOfNo);
            break;
        case 6:
            EXPECT_EQ(MKTP.get(proposalId).numberOfYes, numberOfYes);
            EXPECT_EQ(MKTP.get(proposalId).numberOfNo, numberOfNo);
            break;
        case 7:
            EXPECT_EQ(AlloP.get(proposalId).numberOfYes, numberOfYes);
            EXPECT_EQ(AlloP.get(proposalId).numberOfNo, numberOfNo);
            break;
        default:
            break;
        }
    }

    void POST_INCOMING_TRANSFER_checker(uint64 distributedAmount, uint32 epoch, uint32 shareIndex)
    {
        EXPECT_EQ(distributedAmount, totalEpochRevenue);
        EXPECT_EQ(distributedAmount, totalHistoryRevenue);
        EXPECT_EQ(distributedAmount, revenueInQcapPerEpoch.get(epoch));
        EXPECT_EQ(distributedAmount, revenuePerShare.get(shareIndex));
    }

    void endEpochChecker(uint64 qxDistributedAmount)
    {
        EXPECT_EQ(reinvestingFund, div(qxDistributedAmount * reinvestingPermille, 1000ULL));
        EXPECT_EQ(fundForBurn, div(qxDistributedAmount * qcapBurnPermille, 1000ULL));
        EXPECT_EQ(totalEpochRevenue + proposalCreateFund, getBalance(QVAULT_CONTRACT_ID));
    }

    void test()
    {
        EXPECT_EQ(fundForBurn, 34523);
        EXPECT_EQ(totalEpochRevenue, 0);
    }
};

class QXChecker : public QX, public QX::StateData
{
public:
    uint64 getDistributedAmountInEndTick()
    {
        return _distributedAmount;
    }
};

class ContractTestingQvault : protected ContractTesting
{
public:
    ContractTestingQvault()
    {
        initEmptySpectrum();
        initEmptyUniverse();
        INIT_CONTRACT(QVAULT);
        callSystemProcedure(QVAULT_CONTRACT_INDEX, INITIALIZE);
        INIT_CONTRACT(QX);
        callSystemProcedure(QX_CONTRACT_INDEX, INITIALIZE);
    }

    QVAULTChecker* getState()
    {
        return (QVAULTChecker*)contractStates[QVAULT_CONTRACT_INDEX];
    }

    QXChecker* qxGetState()
    {
        return (QXChecker*)contractStates[QX_CONTRACT_INDEX];
    }

    void endEpoch(bool expectSuccess = true)
    {
        callSystemProcedure(QVAULT_CONTRACT_INDEX, END_EPOCH, expectSuccess);
    }

    void beginEpoch(bool expectSuccess = true)
    {
        callSystemProcedure(QVAULT_CONTRACT_INDEX, BEGIN_EPOCH, expectSuccess);
    }

    void qxEndTick(bool expectSuccess = true)
    {
        callSystemProcedure(QX_CONTRACT_INDEX, END_TICK, expectSuccess);
    }

    QVAULT::getData_output getData() const
    {
        QVAULT::getData_input input;
        QVAULT::getData_output output;

        callFunction(QVAULT_CONTRACT_INDEX, 1, input, output);
        return output;
    }

    QVAULT::getStakedAmountAndVotingPower_output getStakedAmountAndVotingPower(id address) const
    {
        QVAULT::getStakedAmountAndVotingPower_input input;
        QVAULT::getStakedAmountAndVotingPower_output output;

        input.address = address;

        callFunction(QVAULT_CONTRACT_INDEX, 2, input, output);
        return output;
    }

    QVAULT::getGP_output getGP(uint32 proposalId) const
    {
        QVAULT::getGP_input input;
        QVAULT::getGP_output output;

        input.proposalId = proposalId;

        callFunction(QVAULT_CONTRACT_INDEX, 3, input, output);
        return output;
    }

    QVAULT::getQCP_output getQCP(uint32 proposalId) const
    {
        QVAULT::getQCP_input input;
        QVAULT::getQCP_output output;

        input.proposalId = proposalId;

        callFunction(QVAULT_CONTRACT_INDEX, 4, input, output);
        return output;
    }

    QVAULT::getIPOP_output getIPOP(uint32 proposalId) const
    {
        QVAULT::getIPOP_input input;
        QVAULT::getIPOP_output output;

        input.proposalId = proposalId;

        callFunction(QVAULT_CONTRACT_INDEX, 5, input, output);
        return output;
    }

    QVAULT::getQEarnP_output getQEarnP(uint32 proposalId) const
    {
        QVAULT::getQEarnP_input input;
        QVAULT::getQEarnP_output output;

        input.proposalId = proposalId;

        callFunction(QVAULT_CONTRACT_INDEX, 6, input, output);
        return output;
    }

    QVAULT::getFundP_output getFundP(uint32 proposalId) const
    {
        QVAULT::getFundP_input input;
        QVAULT::getFundP_output output;

        input.proposalId = proposalId;

        callFunction(QVAULT_CONTRACT_INDEX, 7, input, output);
        return output;
    }

    QVAULT::getMKTP_output getMKTP(uint32 proposalId) const
    {
        QVAULT::getMKTP_input input;
        QVAULT::getMKTP_output output;

        input.proposalId = proposalId;

        callFunction(QVAULT_CONTRACT_INDEX, 8, input, output);
        return output;
    }

    QVAULT::getAlloP_output getAlloP(uint32 proposalId) const
    {
        QVAULT::getAlloP_input input;
        QVAULT::getAlloP_output output;

        input.proposalId = proposalId;

        callFunction(QVAULT_CONTRACT_INDEX, 9, input, output);
        return output;
    }

    QVAULT::getIdentitiesHvVtPw_output getIdentitiesHvVtPw(uint32 offset, uint32 count) const
    {
        QVAULT::getIdentitiesHvVtPw_input input;
        QVAULT::getIdentitiesHvVtPw_output output;

        input.count = count;
        input.offset = offset;

        callFunction(QVAULT_CONTRACT_INDEX, 11, input, output);
        return output;
    }

    QVAULT::ppCreationPower_output ppCreationPower(id address) const
    {
        QVAULT::ppCreationPower_input input;
        QVAULT::ppCreationPower_output output;

        input.address = address;

        callFunction(QVAULT_CONTRACT_INDEX, 12, input, output);
        return output;
    }

    QVAULT::getQcapBurntAmountInLastEpoches_output getQcapBurntAmountInLastEpoches(uint32 numberOfLastEpoches) const
    {
        QVAULT::getQcapBurntAmountInLastEpoches_input input;
        QVAULT::getQcapBurntAmountInLastEpoches_output output;

        input.numberOfLastEpoches = numberOfLastEpoches;

        callFunction(QVAULT_CONTRACT_INDEX, 13, input, output);
        return output;
    }

    QVAULT::getAmountToBeSoldPerYear_output getAmountToBeSoldPerYear(uint32 year) const
    {
        QVAULT::getAmountToBeSoldPerYear_input input;
        QVAULT::getAmountToBeSoldPerYear_output output;

        input.year = year;

        callFunction(QVAULT_CONTRACT_INDEX, 14, input, output);
        return output;
    }

    QVAULT::getTotalRevenueInQcap_output getTotalRevenueInQcap() const
    {
        QVAULT::getTotalRevenueInQcap_input input;
        QVAULT::getTotalRevenueInQcap_output output;

        callFunction(QVAULT_CONTRACT_INDEX, 15, input, output);
        return output;
    }

    QVAULT::getRevenueInQcapPerEpoch_output getRevenueInQcapPerEpoch(uint32 epoch) const
    {
        QVAULT::getRevenueInQcapPerEpoch_input input;
        QVAULT::getRevenueInQcapPerEpoch_output output;

        input.epoch = epoch;

        callFunction(QVAULT_CONTRACT_INDEX, 16, input, output);
        return output;
    }

    QVAULT::getRevenuePerShare_output getRevenuePerShare(uint32 contractIndex) const
    {
        QVAULT::getRevenuePerShare_input input;
        QVAULT::getRevenuePerShare_output output;

        input.contractIndex = contractIndex;

        callFunction(QVAULT_CONTRACT_INDEX, 17, input, output);
        return output;
    }

    QVAULT::getAmountOfShareQvaultHold_output getAmountOfShareQvaultHold(Asset assetInfo) const
    {
        QVAULT::getAmountOfShareQvaultHold_input input;
        QVAULT::getAmountOfShareQvaultHold_output output;

        input.assetInfo = assetInfo;

        callFunction(QVAULT_CONTRACT_INDEX, 18, input, output);
        return output;
    }

    QVAULT::getNumberOfHolderAndAvgAm_output getNumberOfHolderAndAvgAm() const
    {
        QVAULT::getNumberOfHolderAndAvgAm_input input;
        QVAULT::getNumberOfHolderAndAvgAm_output output;

        callFunction(QVAULT_CONTRACT_INDEX, 19, input, output);
        return output;
    }

    QVAULT::getAmountForQearnInUpcomingEpoch_output getAmountForQearnInUpcomingEpoch(uint32 epoch) const
    {
        QVAULT::getAmountForQearnInUpcomingEpoch_input input;
        QVAULT::getAmountForQearnInUpcomingEpoch_output output;

        input.epoch = epoch;

        callFunction(QVAULT_CONTRACT_INDEX, 20, input, output);
        return output;
    }

    QVAULT::getStakeEpochFlag_output getStakeEpochFlag(id address) const
    {
        QVAULT::getStakeEpochFlag_input input;
        QVAULT::getStakeEpochFlag_output output;

        input.userID = address;

        callFunction(QVAULT_CONTRACT_INDEX, 22, input, output);
        return output;
    }

    uint32 stake(const id& address, uint32 amount)
    {
        QVAULT::stake_input input;
        QVAULT::stake_output output;

        input.amount = amount;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 1, input, output, address, 0);

        return output.returnCode;
    }

    uint32 unStake(const id& address, uint32 amount)
    {
        QVAULT::unStake_input input{amount};
        QVAULT::unStake_output output;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 2, input, output, address, QVAULT_UNSTAKE_FEE);

        return output.returnCode;
    }

    uint32 submitGP(const id& address)
    {
        QVAULT::submitGP_input input;
        QVAULT::submitGP_output output;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 3, input, output, address, QVAULT_PROPOSAL_FEE);

        return output.returnCode;
    }

    uint32 submitQCP(const id& address, uint32 newQuorumPercent)
    {
        QVAULT::submitQCP_input input;
        QVAULT::submitQCP_output output;

        input.newQuorumPercent = newQuorumPercent;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 4, input, output, address, QVAULT_PROPOSAL_FEE);

        return output.returnCode;
    }

    uint32 submitIPOP(const id& address, uint32 ipoContractIndex)
    {
        QVAULT::submitIPOP_input input;
        QVAULT::submitIPOP_output output;

        input.ipoContractIndex = ipoContractIndex;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 5, input, output, address, QVAULT_PROPOSAL_FEE);

        return output.returnCode;
    }

    uint32 submitQEarnP(const id& address, uint64 amountPerEpoch, uint32 numberOfEpoch)
    {
        QVAULT::submitQEarnP_input input;
        QVAULT::submitQEarnP_output output;

        input.amountPerEpoch = amountPerEpoch;
        input.numberOfEpoch = numberOfEpoch;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 6, input, output, address, QVAULT_PROPOSAL_FEE);

        return output.returnCode;
    }

    uint32 submitFundP(const id& address, uint32 amountOfQcap, uint64 priceOfOneQcap)
    {
        QVAULT::submitFundP_input input;
        QVAULT::submitFundP_output output;

        input.amountOfQcap = amountOfQcap;
        input.priceOfOneQcap = priceOfOneQcap;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 7, input, output, address, QVAULT_PROPOSAL_FEE);

        return output.returnCode;
    }

    uint32 submitMKTP(const id& address, uint32 amountOfQcap, uint64 amountOfQubic, uint64 shareName, uint32 indexOfShare, uint32 amountOfShare)
    {
        QVAULT::submitMKTP_input input;
        QVAULT::submitMKTP_output output;

        input.amountOfQcap = amountOfQcap;
        input.amountOfQubic = amountOfQubic;
        input.amountOfShare = amountOfShare;
        input.indexOfShare = indexOfShare;
        input.shareName = shareName;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 8, input, output, address, QVAULT_PROPOSAL_FEE);

        return output.returnCode;
    }

    uint32 submitAlloP(const id& address, uint32 reinvested, uint32 burn, uint32 distribute)
    {
        QVAULT::submitAlloP_input input;
        QVAULT::submitAlloP_output output;

        input.reinvested = reinvested;
        input.burn = burn ;
        input.distribute = distribute;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 9, input, output, address, QVAULT_PROPOSAL_FEE);

        return output.returnCode;
    }

    uint32 voteInProposal(const id& address, uint64 priceOfIPO, uint32 proposalType, uint32 proposalId, bit yes)
    {
        QVAULT::voteInProposal_input input;
        QVAULT::voteInProposal_output output;

        input.priceOfIPO = priceOfIPO;
        input.proposalType = proposalType;
        input.proposalId = proposalId;
        input.yes = yes;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 11, input, output, address, 0);

        return output.returnCode;
    }

    uint32 buyQcap(const id& address, uint32 amount, uint64 fund)
    {
        QVAULT::buyQcap_input input;
        QVAULT::buyQcap_output output;

        input.amount = amount;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 12, input, output, address, fund);

        return output.returnCode;
    }

    uint64 TransferShareManagementRights(const id& address, Asset asset, sint64 numberOfShares, uint32 newManagingContractIndex)
    {
        QVAULT::TransferShareManagementRights_input input;
        QVAULT::TransferShareManagementRights_output output;

        input.asset.assetName = QVAULT_QCAP_ASSETNAME;
        input.asset.issuer = QVAULT_QCAP_ISSUER;
        input.numberOfShares = numberOfShares;
        input.newManagingContractIndex = newManagingContractIndex;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 13, input, output, address, 100);

        return output.transferredNumberOfShares;
    }

    sint64 QXTransferShareManagementRights(const id& issuer, uint64 assetName, uint32 newManagingContractIndex, sint64 numberOfShares, id currentOwner)
    {
        QX::TransferShareManagementRights_input input;
        QX::TransferShareManagementRights_output output;

        input.asset.assetName = assetName;
        input.asset.issuer = issuer;
        input.newManagingContractIndex = newManagingContractIndex;
        input.numberOfShares = numberOfShares;

        invokeUserProcedure(QX_CONTRACT_INDEX, 9, input, output, currentOwner, 0);

        return output.transferredNumberOfShares;
    }

    sint64 issueAsset(const id& issuer, uint64 assetName, sint64 numberOfShares, uint64 unitOfMeasurement, sint8 numberOfDecimalPlaces)
    {
        QX::IssueAsset_input input{ assetName, numberOfShares, unitOfMeasurement, numberOfDecimalPlaces };
        QX::IssueAsset_output output;
        invokeUserProcedure(QX_CONTRACT_INDEX, 1, input, output, issuer, QVAULT_ISSUE_ASSET_FEE);
        return output.issuedNumberOfShares;
    }

    sint64 TransferShareOwnershipAndPossession(const id& issuer, uint64 assetName, sint64 numberOfShares, id oldOwnerAndPossessor, id newOwnerAndPossesor)
    {
        QX::TransferShareOwnershipAndPossession_input input;
        QX::TransferShareOwnershipAndPossession_output output;

        input.assetName = assetName;
        input.issuer = issuer;
        input.newOwnerAndPossessor = newOwnerAndPossesor;
        input.numberOfShares = numberOfShares;

        invokeUserProcedure(QX_CONTRACT_INDEX, 2, input, output, oldOwnerAndPossessor, QVAULT_TOKEN_TRANSFER_FEE);

        return output.transferredNumberOfShares;
    }

    QX::AddToAskOrder_output AddToAskOrder(const id& issuer, uint64 assetName, sint64 price, sint64 numberOfShares, id user)
    {
        QX::AddToAskOrder_input input{ issuer, assetName, price, numberOfShares};
        QX::AddToAskOrder_output output;
        invokeUserProcedure(QX_CONTRACT_INDEX, 5, input, output, user, 0);
        return output;
    }

    QX::AssetAskOrders_output AssetAskOrders(const id& issuer, uint64 assetName, uint64 offset)
    {
        QX::AssetAskOrders_input input{ issuer, assetName, offset};
        QX::AssetAskOrders_output output;
        callFunction(QX_CONTRACT_INDEX, 2, input, output);
        return output;
    }

    uint32 submitTreasuryP(const id& address, uint8 action, const id& destination, const id& assetIssuer, uint64 assetName, uint64 amountOfQubic, sint64 numberOfShares, sint64 pricePerShare)
    {
        QVAULT::submitTreasuryP_input input;
        QVAULT::submitTreasuryP_output output;
        setMemory(input, 0);

        input.action = action;
        input.destination = destination;
        input.assetIssuer = assetIssuer;
        input.assetName = assetName;
        input.amountOfQubic = amountOfQubic;
        input.numberOfShares = numberOfShares;
        input.pricePerShare = pricePerShare;

        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 14, input, output, address, QVAULT_PROPOSAL_FEE);

        return output.returnCode;
    }

    QVAULT::getTreasuryP_output getTreasuryP(uint32 proposalId) const
    {
        QVAULT::getTreasuryP_input input;
        QVAULT::getTreasuryP_output output;
        input.proposalId = proposalId;
        callFunction(QVAULT_CONTRACT_INDEX, 23, input, output);
        return output;
    }
};


TEST(TestContractQvault, testingAllProceduresAndFunctions)
{
    system.epoch = 138;
    ContractTestingQvault QvaultV2;

    uint64 qcapAssetName = assetNameFromString("QCAP");

    increaseEnergy(QVAULT_QCAP_ISSUER, QVAULT_ISSUE_ASSET_FEE);
    EXPECT_EQ(QvaultV2.issueAsset(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_QCAP_MAX_SUPPLY, 0, 0), QVAULT_QCAP_MAX_SUPPLY);

    increaseEnergy(QVAULT_QCAP_ISSUER, QVAULT_TOKEN_TRANSFER_FEE);
    EXPECT_EQ(QvaultV2.TransferShareOwnershipAndPossession(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_QCAP_MAX_SUPPLY, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID), QVAULT_QCAP_MAX_SUPPLY);

    auto stakers = getRandomUsers(QVAULT_QCAP_MAX_HOLDERS, QVAULT_QCAP_MAX_HOLDERS);

    uint32 maxTransferAmount = QVAULT_QCAP_SOLD_AMOUNT;
    for (const auto& user : stakers)
    {
        uint32 amountTransfer = (uint32)random(1000ULL, 100000ULL);
        if(maxTransferAmount < amountTransfer)
        {
            break;
        }
        maxTransferAmount -= amountTransfer;
        increaseEnergy(QVAULT_CONTRACT_ID, QVAULT_TOKEN_TRANSFER_FEE);

        EXPECT_EQ(QvaultV2.TransferShareOwnershipAndPossession(QVAULT_QCAP_ISSUER, qcapAssetName, amountTransfer, QVAULT_CONTRACT_ID, user), amountTransfer);

        increaseEnergy(user, QVAULT_UNSTAKE_FEE);
        EXPECT_EQ(QvaultV2.QXTransferShareManagementRights(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_CONTRACT_INDEX, amountTransfer, user), amountTransfer);
        EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, user, user, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), amountTransfer);
        if (user == stakers[0])
        {
            EXPECT_EQ(QvaultV2.stake(user, QVAULT_MIN_STAKE_AMOUNT - 1), QVAULT_INVALID_STAKE_AMOUNT);
            EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), 0);
        }
        else
        {
            EXPECT_EQ(QvaultV2.getStakeEpochFlag(user).flag, 0);
            EXPECT_EQ(QvaultV2.stake(user, amountTransfer), 0);
            EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), amountTransfer);
            EXPECT_EQ(QvaultV2.getStakeEpochFlag(user).flag, 1);
            EXPECT_EQ(QvaultV2.unStake(user, amountTransfer), 0);
            EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, user, user, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), amountTransfer);
            EXPECT_EQ(QvaultV2.getStakeEpochFlag(user).flag, 3);
        }
    }

    QvaultV2.TransferShareOwnershipAndPossession(QVAULT_QCAP_ISSUER, qcapAssetName, 10100, QVAULT_CONTRACT_ID, stakers[0]);
    EXPECT_EQ(QvaultV2.stake(stakers[0], 10000), 0);
    EXPECT_EQ(QvaultV2.stake(stakers[0], 100), QVAULT_STAKE_ALREADY_DONE);

    QvaultV2.endEpoch();
    system.epoch++;

        increaseEnergy(stakers[0], QVAULT_PROPOSAL_FEE);
        EXPECT_EQ(QvaultV2.submitGP(stakers[0]), QVAULT_SUCCESS);
        EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE);
        QvaultV2.getState()->submitGPChecker(0, stakers[0]);

        increaseEnergy(stakers[0], QVAULT_PROPOSAL_FEE);
        EXPECT_EQ(QvaultV2.submitQCP(stakers[0], 500), QVAULT_SUCCESS);
        EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 2);
        QvaultV2.getState()->submitQCPChecker(0, stakers[0], 500);

        increaseEnergy(stakers[0], QVAULT_PROPOSAL_FEE);
        EXPECT_EQ(QvaultV2.submitIPOP(stakers[0], 15), QVAULT_INSUFFICIENT_FUND);
        EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 2);
        // QvaultV2.getState()->submitIPOPChecker(0, stakers[0], 15);

        increaseEnergy(stakers[0], QVAULT_PROPOSAL_FEE);
        EXPECT_EQ(QvaultV2.submitQEarnP(stakers[0], 1000000000, 10), QVAULT_INSUFFICIENT_FUND);
        EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 2);
        // QvaultV2.getState()->submitQEarnPChecker(0, stakers[0], 1000000000, 10);

    updateTime();
    updateQpiTime();

    setMemory(utcTime, 0);
    utcTime.Year = 2025;
    utcTime.Month = 1;
    utcTime.Day = 3;
    utcTime.Hour = 0;
    updateQpiTime();

    increaseEnergy(stakers[0], 10000000);
    EXPECT_EQ(QvaultV2.submitFundP(stakers[0], QVAULT_2025MAX_QCAP_SALE_AMOUNT, 100000), QVAULT_OVERFLOW_SALE_AMOUNT);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 2);
    EXPECT_EQ(QvaultV2.submitFundP(stakers[0], QVAULT_2025MAX_QCAP_SALE_AMOUNT - QVAULT_QCAP_SOLD_AMOUNT, 100000), QVAULT_SUCCESS);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 3);
    QvaultV2.getState()->submitFundPChecker(0, stakers[0], QVAULT_2025MAX_QCAP_SALE_AMOUNT - QVAULT_QCAP_SOLD_AMOUNT, 100000);

    setMemory(utcTime, 0);
    utcTime.Year = 2026;
    utcTime.Month = 1;
    utcTime.Day = 3;
    utcTime.Hour = 0;
    updateQpiTime();

    increaseEnergy(stakers[0], 10000000);
    EXPECT_EQ(QvaultV2.submitFundP(stakers[0], QVAULT_2026MAX_QCAP_SALE_AMOUNT, 100000), QVAULT_OVERFLOW_SALE_AMOUNT);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 3);
    EXPECT_EQ(QvaultV2.submitFundP(stakers[0], QVAULT_2026MAX_QCAP_SALE_AMOUNT - QVAULT_QCAP_SOLD_AMOUNT, 100000), QVAULT_SUCCESS);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 4);
    QvaultV2.getState()->submitFundPChecker(1, stakers[0], QVAULT_2026MAX_QCAP_SALE_AMOUNT - QVAULT_QCAP_SOLD_AMOUNT, 100000);

    setMemory(utcTime, 0);
    utcTime.Year = 2027;
    utcTime.Month = 1;
    utcTime.Day = 3;
    utcTime.Hour = 0;
    updateQpiTime();

    increaseEnergy(stakers[0], 10000000);
    EXPECT_EQ(QvaultV2.submitFundP(stakers[0], QVAULT_2027MAX_QCAP_SALE_AMOUNT, 100000), QVAULT_OVERFLOW_SALE_AMOUNT);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 4);
    EXPECT_EQ(QvaultV2.submitFundP(stakers[0], QVAULT_2027MAX_QCAP_SALE_AMOUNT - QVAULT_QCAP_SOLD_AMOUNT, 100000), QVAULT_SUCCESS);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 5);
    QvaultV2.getState()->submitFundPChecker(2, stakers[0], QVAULT_2027MAX_QCAP_SALE_AMOUNT - QVAULT_QCAP_SOLD_AMOUNT, 100000);

    setMemory(utcTime, 0);
    utcTime.Year = 2028;
    utcTime.Month = 1;
    utcTime.Day = 3;
    utcTime.Hour = 0;
    updateQpiTime();

    increaseEnergy(stakers[0], 10000000);
    EXPECT_EQ(QvaultV2.submitFundP(stakers[0], QVAULT_QCAP_MAX_SUPPLY, 100000), QVAULT_OVERFLOW_SALE_AMOUNT);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 5);
    EXPECT_EQ(QvaultV2.submitFundP(stakers[0], QVAULT_QCAP_MAX_SUPPLY - QVAULT_QCAP_SOLD_AMOUNT, 100000), QVAULT_SUCCESS);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 6);
    QvaultV2.getState()->submitFundPChecker(3, stakers[0], QVAULT_QCAP_MAX_SUPPLY - QVAULT_QCAP_SOLD_AMOUNT, 100000);

    increaseEnergy(stakers[0], 10000000);
    EXPECT_EQ(QvaultV2.submitMKTP(stakers[0], 10000, 1000000000, assetNameFromString("QX"), 1, 5), QVAULT_NOT_TRANSFERRED_SHARE);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 6);
    // QvaultV2.getState()->submitMKTPChecker(0, stakers[0], 10000, 1000000000, assetNameFromString("QX"), 1, 5);

    increaseEnergy(stakers[0], 10000000);
    EXPECT_EQ(QvaultV2.submitAlloP(stakers[0], 450, 120, 400), QVAULT_NOT_IN_TIME);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 6);

    setMemory(utcTime, 0);
    utcTime.Year = 2029;
    utcTime.Month = 1;
    utcTime.Day = 3;
    utcTime.Hour = 0;
    updateQpiTime();

    EXPECT_EQ(QvaultV2.submitAlloP(stakers[0], 450, 120, 400), QVAULT_SUCCESS);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 7);

    // Allocation percentages whose true sum is 2^32 + 970 must be rejected as
    // QVAULT_NOT_FAIR: reinvested = 2^32-1, burn = 0, distribute = 971 wraps to
    // exactly QVAULT_SUM_OF_ALLOCATION_PERCENTAGES (970) under 32-bit addition,
    // so the sum must be evaluated in 64-bit. The rejected call refunds the
    // proposal fee, so the contract balance stays unchanged.
    increaseEnergy(stakers[0], 10000000);
    EXPECT_EQ(QvaultV2.submitAlloP(stakers[0], 4294967295u, 0, 971), QVAULT_NOT_FAIR);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 7);

    setMemory(utcTime, 0);
    utcTime.Year = 2029;
    utcTime.Month = 1;
    utcTime.Day = 3;
    utcTime.Hour = 0;
    updateQpiTime();

    EXPECT_EQ(QvaultV2.submitAlloP(stakers[0], 450, 120, 400), QVAULT_SUCCESS);
    EXPECT_EQ(getBalance(QVAULT_CONTRACT_ID), QVAULT_PROPOSAL_FEE * 8);
    QvaultV2.getState()->submitAlloPChecker(0, stakers[0], 450, 120, 400);

    EXPECT_EQ(QvaultV2.voteInProposal(stakers[0], 100000000, 1, 0, 1), QVAULT_SUCCESS);
    QvaultV2.getState()->voteInProposalChecker(0, 1, 10000, 0);
    EXPECT_EQ(QvaultV2.voteInProposal(stakers[0], 100000000, 1, 0, 1), QVAULT_SAME_DECISION);
    QvaultV2.getState()->voteInProposalChecker(0, 1, 10000, 0);
    EXPECT_EQ(QvaultV2.voteInProposal(stakers[0], 100000000, 1, 0, 0), QVAULT_SUCCESS);
    QvaultV2.getState()->voteInProposalChecker(0, 1, 0, 10000);
    EXPECT_EQ(QvaultV2.voteInProposal(stakers[0], 100000000, 1, 0, 1), QVAULT_OVERFLOW_VOTES);
    QvaultV2.getState()->voteInProposalChecker(0, 1, 0, 10000);

    EXPECT_EQ(QvaultV2.voteInProposal(stakers[0], 100000000, 2, 0, 1), QVAULT_SUCCESS);
    QvaultV2.getState()->voteInProposalChecker(0, 2, 10000, 0);

    EXPECT_EQ(QvaultV2.voteInProposal(stakers[0], 100000000, 5, 0, 1), QVAULT_SUCCESS);
    QvaultV2.getState()->voteInProposalChecker(0, 5, 10000, 0);

    EXPECT_EQ(QvaultV2.voteInProposal(stakers[0], 100000000, 7, 0, 1), QVAULT_SUCCESS);
    QvaultV2.getState()->voteInProposalChecker(0, 7, 10000, 0);

    Asset qcapShare;
    qcapShare.assetName = qcapAssetName;
    qcapShare.issuer = QVAULT_QCAP_ISSUER;

    EXPECT_EQ(QvaultV2.QXTransferShareManagementRights(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_CONTRACT_INDEX, 10000, QVAULT_CONTRACT_ID), 10000);
    increaseEnergy(QVAULT_QCAP_ISSUER, 1000000);
    increaseEnergy(QVAULT_CONTRACT_ID, QVAULT_TOKEN_TRANSFER_FEE);
    EXPECT_EQ(QvaultV2.TransferShareManagementRights(QVAULT_CONTRACT_ID, qcapShare, 10000, QX_CONTRACT_INDEX), 10000);

    std::vector<std::pair<m256i, unsigned int>> qxSharesHolers{{QVAULT_CONTRACT_ID, 676}};
    issueContractShares(QX_CONTRACT_INDEX, qxSharesHolers);

    std::vector<std::pair<m256i, unsigned int>> qvaultSharesHolers{ {stakers[1], 1}, {stakers[2], 675}};
    issueContractShares(QVAULT_CONTRACT_INDEX, qvaultSharesHolers);

    increaseEnergy(stakers[1], 10000000);
    EXPECT_EQ(QvaultV2.submitGP(stakers[1]), QVAULT_SUCCESS);
    QvaultV2.getState()->submitGPChecker(1, stakers[1]);
    
    for (uint32 t = 0 ; t < 100; t++)
    {
        uint64 newAssetName;
        char strAssetName[6];
        for (uint32 r = 0 ; r < 5; r++)
        {
            strAssetName[r] = 'A' + (uint32)random(0, 25);
        }
        strAssetName[5] = 0;
        newAssetName = assetNameFromString(strAssetName);
        increaseEnergy(stakers[2], 1000000000);
        QvaultV2.issueAsset(stakers[2], newAssetName, 1000000000, 0, 0);
    }

    QvaultV2.qxEndTick();
    uint64 qxDistributedAmount = QvaultV2.qxGetState()->getDistributedAmountInEndTick();
    
    QvaultV2.getState()->POST_INCOMING_TRANSFER_checker(qxDistributedAmount, system.epoch, QX_CONTRACT_INDEX);

    QvaultV2.endEpoch();

    QvaultV2.getState()->endEpochChecker(qxDistributedAmount);

    QvaultV2.TransferShareOwnershipAndPossession(QVAULT_QCAP_ISSUER, qcapAssetName, 30000, QVAULT_CONTRACT_ID, stakers[0]);

    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, stakers[0], stakers[0], QX_CONTRACT_INDEX, QX_CONTRACT_INDEX), 30000);
    QvaultV2.AddToAskOrder(QVAULT_QCAP_ISSUER, qcapAssetName, 1000, 30000, stakers[0]);

    system.epoch += 2;
    
    QvaultV2.beginEpoch();

    for (uint32 t = 0 ; t < 100; t++)
    {
        uint64 newAssetName;
        char strAssetName[6];
        for (uint32 r = 0 ; r < 5; r++)
        {
            strAssetName[r] = 'B' + (uint32)random(0, 24);
        }
        strAssetName[5] = 0;
        newAssetName = assetNameFromString(strAssetName);
        increaseEnergy(stakers[2], 1000000000);
        QvaultV2.issueAsset(stakers[2], newAssetName, 1000000000, 0, 0);
    }

    QvaultV2.qxEndTick();

    QvaultV2.endEpoch();

    system.epoch++;
    uint64 currentNumberOfQcapInQvault = numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QX_CONTRACT_INDEX, QX_CONTRACT_INDEX) + numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX);
    QvaultV2.beginEpoch(); 

    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, stakers[0], stakers[0], QX_CONTRACT_INDEX, QX_CONTRACT_INDEX), 0);
    EXPECT_EQ(currentNumberOfQcapInQvault, numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QX_CONTRACT_INDEX, QX_CONTRACT_INDEX) + numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX));

    increaseEnergy(stakers[3], 100000000);
    uint64 tmpAmount = numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, stakers[3], stakers[3], QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX);
    QvaultV2.buyQcap(stakers[3], 100, 10000000);
    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, stakers[3], stakers[3], QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), tmpAmount + 100);
}
TEST(TestContractQvault, frozenStakeMovedToEscrowOnce)
{
    system.epoch = 138;
    ContractTestingQvault qvault;

    uint64 qcapAssetName = assetNameFromString("QCAP");

    increaseEnergy(QVAULT_QCAP_ISSUER, QVAULT_ISSUE_ASSET_FEE);
    EXPECT_EQ(qvault.issueAsset(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_QCAP_MAX_SUPPLY, 0, 0), QVAULT_QCAP_MAX_SUPPLY);
    increaseEnergy(QVAULT_QCAP_ISSUER, QVAULT_TOKEN_TRANSFER_FEE);
    EXPECT_EQ(qvault.TransferShareOwnershipAndPossession(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_QCAP_MAX_SUPPLY, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID), QVAULT_QCAP_MAX_SUPPLY);

    m256i frozenStaker;
    getPublicKeyFromIdentity(gFrozenIdentities[0], frozenStaker.m256i_u8);
    ASSERT_TRUE(isFrozenAddress(frozenStaker));
    const id normalStaker = getUser(1);
    ASSERT_FALSE(isFrozenAddress(normalStaker));

    // the escrow id hardcoded in QVAULT must be the core's FROZEN_ESCROW
    const id escrow = ID(_X, _M, _G, _U, _N, _W, _J, _Y, _L, _L, _U, _U, _Z, _A, _N, _F, _E, _W, _J, _N, _N, _I, _D, _M, _X, _N, _S, _D, _M, _P, _O, _V, _F, _A, _H, _W, _O, _E, _R, _K, _D, _D, _A, _W, _D, _R, _E, _Z, _L, _A, _N, _H, _T, _F, _J, _B);
    ASSERT_EQ(escrow, frozenEscrowPublicKey);

    const uint32 frozenAmount = 5000;
    const uint32 normalAmount = 3000;
    const id users[2] = { frozenStaker, normalStaker };
    const uint32 amounts[2] = { frozenAmount, normalAmount };
    for (int i = 0; i < 2; i++)
    {
        increaseEnergy(QVAULT_CONTRACT_ID, QVAULT_TOKEN_TRANSFER_FEE);
        EXPECT_EQ(qvault.TransferShareOwnershipAndPossession(QVAULT_QCAP_ISSUER, qcapAssetName, amounts[i], QVAULT_CONTRACT_ID, users[i]), amounts[i]);
        increaseEnergy(users[i], QVAULT_UNSTAKE_FEE);
        EXPECT_EQ(qvault.QXTransferShareManagementRights(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_CONTRACT_INDEX, amounts[i], users[i]), amounts[i]);
        EXPECT_EQ(qvault.stake(users[i], amounts[i]), QVAULT_SUCCESS);
    }

    // end of epoch copies stakers into the voting power snapshot
    qvault.endEpoch();
    EXPECT_EQ(qvault.getState()->numberOfStaker, 2);
    EXPECT_EQ(qvault.getState()->numberOfVotingPower, 2);
    EXPECT_EQ(qvault.getState()->totalVotingPower, frozenAmount + normalAmount);
    EXPECT_EQ(qvault.getState()->totalStakedQcapAmount, frozenAmount + normalAmount);

    // any epoch other than QVAULT_FROZEN_STAKE_ESCROW_EPOCH: nothing moves
    system.epoch = QVAULT_FROZEN_STAKE_ESCROW_EPOCH - 1;
    qvault.beginEpoch();
    EXPECT_EQ(qvault.getState()->numberOfStaker, 2);
    EXPECT_EQ(qvault.getState()->totalStakedQcapAmount, frozenAmount + normalAmount);
    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, escrow, escrow, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), 0);

    const sint64 qvaultQcapBefore = numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX);

    system.epoch = QVAULT_FROZEN_STAKE_ESCROW_EPOCH;
    qvault.beginEpoch();

    // frozen stake is now in FROZEN_ESCROW, still managed by QVAULT
    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, escrow, escrow, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), frozenAmount);
    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), qvaultQcapBefore - frozenAmount);

    // only the normal staker is left in both lists
    QVAULTChecker* state = qvault.getState();
    EXPECT_EQ(state->numberOfStaker, 1);
    EXPECT_EQ(state->staker.get(0).stakerAddress, normalStaker);
    EXPECT_EQ(state->staker.get(0).amount, normalAmount);
    EXPECT_EQ(state->staker.get(1).amount, 0);
    EXPECT_EQ(state->totalStakedQcapAmount, normalAmount);
    EXPECT_EQ(state->numberOfVotingPower, 1);
    EXPECT_EQ(state->votingPower.get(0).stakerAddress, normalStaker);
    EXPECT_EQ(state->votingPower.get(0).amount, normalAmount);
    EXPECT_EQ(state->votingPower.get(1).amount, 0);
    EXPECT_EQ(state->totalVotingPower, normalAmount);

    // running again in the same epoch changes nothing
    qvault.beginEpoch();
    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, escrow, escrow, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), frozenAmount);
    EXPECT_EQ(state->numberOfStaker, 1);
    EXPECT_EQ(state->totalStakedQcapAmount, normalAmount);

    // the normal staker can still unstake everything
    increaseEnergy(normalStaker, QVAULT_UNSTAKE_FEE);
    EXPECT_EQ(qvault.unStake(normalStaker, normalAmount), QVAULT_SUCCESS);
    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, normalStaker, normalStaker, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), normalAmount);
    EXPECT_EQ(state->numberOfStaker, 0);
    EXPECT_EQ(state->totalStakedQcapAmount, 0);
}

TEST(TestContractQvault, passedProposalsTakeEffectNextEpoch)
{
    const uint32 cutover = QVAULT_FAST_PROPOSAL_EFFECT_EPOCH;
    system.epoch = cutover - 2;
    ContractTestingQvault qvault;
    QVAULTChecker* state = qvault.getState();

    // quorum change proposed before the cutover keeps the old timing (two epochs later)
    QVAULT::QCPInfo qcp;
    setMemory(qcp, 0);
    qcp.proposedEpoch = cutover - 2;
    qcp.newQuorumPercent = 400;
    qcp.result = QVAULT_PROPOSAL_PASSED;
    state->QCP.set(0, qcp);
    state->numberOfQCP = 1;
    const uint32 initialQuorum = state->quorumPercent;
    ASSERT_NE(initialQuorum, 400u);

    system.epoch = cutover - 1;
    qvault.beginEpoch();
    EXPECT_EQ(state->quorumPercent, initialQuorum);
    system.epoch = cutover;
    qvault.beginEpoch();
    EXPECT_EQ(state->quorumPercent, 400u);

    // quorum change and allocation proposed in the cutover epoch apply as soon as the next epoch begins
    qcp.proposedEpoch = cutover;
    qcp.newQuorumPercent = 500;
    state->QCP.set(1, qcp);
    state->numberOfQCP = 2;

    QVAULT::AlloPInfo allo;
    setMemory(allo, 0);
    allo.proposedEpoch = cutover;
    allo.distributed = 950;
    allo.reinvested = 20;
    allo.burnQcap = 0;
    allo.result = QVAULT_PROPOSAL_PASSED;
    state->AlloP.set(0, allo);
    state->numberOfAlloP = 1;
    const uint32 initialDistributed = state->QCAPHolderPermille;
    ASSERT_NE(initialDistributed, 950u);

    // QCAP sale proposed in the cutover epoch opens in the next epoch
    uint64 qcapAssetName = assetNameFromString("QCAP");
    increaseEnergy(QVAULT_QCAP_ISSUER, QVAULT_ISSUE_ASSET_FEE);
    EXPECT_EQ(qvault.issueAsset(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_QCAP_MAX_SUPPLY, 0, 0), QVAULT_QCAP_MAX_SUPPLY);
    increaseEnergy(QVAULT_QCAP_ISSUER, QVAULT_TOKEN_TRANSFER_FEE);
    EXPECT_EQ(qvault.TransferShareOwnershipAndPossession(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_QCAP_MAX_SUPPLY, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID), QVAULT_QCAP_MAX_SUPPLY);

    QVAULT::FundPInfo fund;
    setMemory(fund, 0);
    fund.proposedEpoch = cutover;
    fund.pricePerOneQcap = 100000;
    fund.amountOfQcap = 1000;
    fund.restSaleAmount = 1000;
    fund.result = QVAULT_PROPOSAL_PASSED;
    state->FundP.set(0, fund);
    state->numberOfFundP = 1;

    setMemory(utcTime, 0);
    utcTime.Year = 2026;
    utcTime.Month = 12;
    utcTime.Day = 1;
    updateQpiTime();

    const id buyer = getUser(7);
    const uint32 buyAmount = 100;
    const uint64 payment = fund.pricePerOneQcap * buyAmount;
    increaseEnergy(buyer, payment);
    EXPECT_EQ(qvault.buyQcap(buyer, buyAmount, payment), QVAULT_FAILED);   // still the voting epoch
    EXPECT_EQ(getBalance(buyer), payment);                                  // payment refunded

    system.epoch = cutover + 1;
    qvault.beginEpoch();
    EXPECT_EQ(state->quorumPercent, 500u);
    EXPECT_EQ(state->QCAPHolderPermille, 950u);
    EXPECT_EQ(state->reinvestingPermille, 20u);
    EXPECT_EQ(state->qcapBurnPermille, 0u);

    EXPECT_EQ(qvault.buyQcap(buyer, buyAmount, payment), QVAULT_SUCCESS);
    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, buyer, buyer, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), buyAmount);
    EXPECT_EQ(state->FundP.get(0).restSaleAmount, 1000u - buyAmount);
}

TEST(TestContractQvault, treasuryProposals)
{
    system.epoch = 138;
    ContractTestingQvault qvault;
    QVAULTChecker* state = qvault.getState();

    const uint64 qcapAssetName = assetNameFromString("QCAP");
    increaseEnergy(QVAULT_QCAP_ISSUER, QVAULT_ISSUE_ASSET_FEE);
    EXPECT_EQ(qvault.issueAsset(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_QCAP_MAX_SUPPLY, 0, 0), QVAULT_QCAP_MAX_SUPPLY);
    increaseEnergy(QVAULT_QCAP_ISSUER, QVAULT_TOKEN_TRANSFER_FEE);
    EXPECT_EQ(qvault.TransferShareOwnershipAndPossession(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_QCAP_MAX_SUPPLY, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID), QVAULT_QCAP_MAX_SUPPLY);

    // one staker with 10000 voting power
    const id staker = getUser(1);
    increaseEnergy(QVAULT_CONTRACT_ID, QVAULT_TOKEN_TRANSFER_FEE);
    EXPECT_EQ(qvault.TransferShareOwnershipAndPossession(QVAULT_QCAP_ISSUER, qcapAssetName, 10000, QVAULT_CONTRACT_ID, staker), 10000);
    increaseEnergy(staker, QVAULT_UNSTAKE_FEE);
    EXPECT_EQ(qvault.QXTransferShareManagementRights(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_CONTRACT_INDEX, 10000, staker), 10000);
    EXPECT_EQ(qvault.stake(staker, 10000), QVAULT_SUCCESS);
    qvault.endEpoch();
    system.epoch++;
    ASSERT_EQ(state->totalVotingPower, 10000u);

    // another token received by QVAULT on QX
    const id tokenIssuer = getUser(2);
    const uint64 tokenName = assetNameFromString("TOKEN");
    increaseEnergy(tokenIssuer, QVAULT_ISSUE_ASSET_FEE);
    EXPECT_EQ(qvault.issueAsset(tokenIssuer, tokenName, 1000, 0, 0), 1000);
    increaseEnergy(tokenIssuer, QVAULT_TOKEN_TRANSFER_FEE);
    EXPECT_EQ(qvault.TransferShareOwnershipAndPossession(tokenIssuer, tokenName, 1000, tokenIssuer, QVAULT_CONTRACT_ID), 1000);

    // reinvesting fund backed by real balance
    const uint64 fund = 5000000000ull;
    increaseEnergy(QVAULT_CONTRACT_ID, fund);
    state->reinvestingFund = fund;

    setMemory(utcTime, 0);
    utcTime.Year = 2029;
    utcTime.Month = 1;
    utcTime.Day = 3;
    updateQpiTime();

    const id destination = getUser(3);
    const uint32 qcapSoldBefore = state->qcapSoldAmount;

    // invalid inputs are refused before any proposal is stored
    increaseEnergy(staker, QVAULT_PROPOSAL_FEE * 20);
    EXPECT_EQ(qvault.submitTreasuryP(staker, QVAULT_TREASURY_SELL_ASSET, NULL_ID, QVAULT_QCAP_ISSUER, qcapAssetName, 0, 100, 1000), QVAULT_INPUT_ERROR); // QCAP sales go through FundP
    EXPECT_EQ(qvault.submitTreasuryP(staker, QVAULT_TREASURY_SEND_QU, NULL_ID, NULL_ID, 0, 1000, 0, 0), QVAULT_INPUT_ERROR);                        // no destination
    EXPECT_EQ(qvault.submitTreasuryP(staker, 9, destination, NULL_ID, 0, 1000, 0, 0), QVAULT_INPUT_ERROR);                                         // unknown action
    increaseEnergy(getUser(4), QVAULT_PROPOSAL_FEE);
    EXPECT_EQ(qvault.submitTreasuryP(getUser(4), QVAULT_TREASURY_SEND_QU, destination, NULL_ID, 0, 1000, 0, 0), QVAULT_NO_VOTING_POWER);            // not a staker
    EXPECT_EQ(getBalance(getUser(4)), QVAULT_PROPOSAL_FEE);                                                                                         // fee refunded
    EXPECT_EQ(qvault.getTreasuryP(0).numberOfTreasuryP, 0u);

    EXPECT_EQ(qvault.submitTreasuryP(staker, QVAULT_TREASURY_SEND_QU, destination, NULL_ID, 0, 1000000000ull, 0, 0), QVAULT_SUCCESS);           // 0: release 1B Qu
    EXPECT_EQ(qvault.submitTreasuryP(staker, QVAULT_TREASURY_SEND_QU, destination, NULL_ID, 0, 1000000000000ull, 0, 0), QVAULT_SUCCESS);        // 1: more than the fund
    EXPECT_EQ(qvault.submitTreasuryP(staker, QVAULT_TREASURY_SEND_ASSET, destination, tokenIssuer, tokenName, 0, 300, 0), QVAULT_SUCCESS);       // 2: send 300 TOKEN
    EXPECT_EQ(qvault.submitTreasuryP(staker, QVAULT_TREASURY_SEND_ASSET, destination, QVAULT_QCAP_ISSUER, qcapAssetName, 0, 500, 0), QVAULT_SUCCESS); // 3: send 500 unsold QCAP
    EXPECT_EQ(qvault.submitTreasuryP(staker, QVAULT_TREASURY_SELL_ASSET, NULL_ID, tokenIssuer, tokenName, 0, 200, 1000), QVAULT_SUCCESS);        // 4: sell 200 TOKEN at 1000
    EXPECT_EQ(qvault.submitTreasuryP(staker, QVAULT_TREASURY_SEND_QU, destination, NULL_ID, 0, 7, 0, 0), QVAULT_SUCCESS);                        // 5: voted down
    EXPECT_EQ(qvault.submitTreasuryP(staker, QVAULT_TREASURY_SEND_QU, destination, NULL_ID, 0, 9, 0, 0), QVAULT_SUCCESS);                        // 6: nobody votes
    EXPECT_EQ(qvault.submitTreasuryP(staker, QVAULT_TREASURY_SEND_ASSET, destination, tokenIssuer, tokenName, 0, 5000, 0), QVAULT_SUCCESS);      // 7: more TOKEN than QVAULT has
    EXPECT_EQ(qvault.getTreasuryP(0).numberOfTreasuryP, 8u);
    EXPECT_EQ(qvault.getTreasuryP(2).proposal.result, QVAULT_PROPOSAL_NOT_STARTED);

    for (uint32 i : { 0u, 1u, 2u, 3u, 4u, 7u })
    {
        EXPECT_EQ(qvault.voteInProposal(staker, 0, QVAULT_PROPOSAL_TYPE_TREASURY, i, 1), QVAULT_SUCCESS);
    }
    EXPECT_EQ(qvault.voteInProposal(staker, 0, QVAULT_PROPOSAL_TYPE_TREASURY, 5, 0), QVAULT_SUCCESS);
    EXPECT_EQ(qvault.voteInProposal(staker, 0, QVAULT_PROPOSAL_TYPE_TREASURY, 8, 1), QVAULT_OVERFLOW_PROPOSAL);
    EXPECT_EQ(qvault.getTreasuryP(0).proposal.numberOfYes, 10000u);
    EXPECT_EQ(qvault.getTreasuryP(5).proposal.numberOfNo, 10000u);

    const sint64 qcapInQvaultBefore = numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QX_CONTRACT_INDEX, QX_CONTRACT_INDEX);
    const sint64 stakedQcapBefore = numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX);
    const uint64 transferFee = qvault.qxGetState()->_transferFee;

    qvault.endEpoch();

    // 0: Qu released from the reinvesting fund
    EXPECT_EQ(qvault.getTreasuryP(0).proposal.result, QVAULT_PROPOSAL_PASSED);
    EXPECT_EQ(getBalance(destination), 1000000000);
    // 1: more than the fund, nothing sent
    EXPECT_EQ(qvault.getTreasuryP(1).proposal.result, QVAULT_PROPOSAL_EXECUTION_FAILED);
    // 2: token sent
    EXPECT_EQ(qvault.getTreasuryP(2).proposal.result, QVAULT_PROPOSAL_PASSED);
    EXPECT_EQ(numberOfPossessedShares(tokenName, tokenIssuer, destination, destination, QX_CONTRACT_INDEX, QX_CONTRACT_INDEX), 300);
    // 3: unsold QCAP sent and counted toward the sale limit; staked QCAP untouched
    EXPECT_EQ(qvault.getTreasuryP(3).proposal.result, QVAULT_PROPOSAL_PASSED);
    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, destination, destination, QX_CONTRACT_INDEX, QX_CONTRACT_INDEX), 500);
    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QX_CONTRACT_INDEX, QX_CONTRACT_INDEX), qcapInQvaultBefore - 500);
    EXPECT_EQ(numberOfPossessedShares(qcapAssetName, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_ID, QVAULT_CONTRACT_INDEX, QVAULT_CONTRACT_INDEX), stakedQcapBefore);
    EXPECT_EQ(state->qcapSoldAmount, qcapSoldBefore + 500);
    // 4: ask order placed on QX
    EXPECT_EQ(qvault.getTreasuryP(4).proposal.result, QVAULT_PROPOSAL_PASSED);
    auto asks = qvault.AssetAskOrders(tokenIssuer, tokenName, 0);
    EXPECT_EQ(asks.orders.get(0).entity, QVAULT_CONTRACT_ID);
    EXPECT_EQ(asks.orders.get(0).price, 1000);
    EXPECT_EQ(asks.orders.get(0).numberOfShares, 200);
    // 5: rejected, 6: no quorum, 7: not enough shares
    EXPECT_EQ(qvault.getTreasuryP(5).proposal.result, QVAULT_PROPOSAL_REJECTED);
    EXPECT_EQ(qvault.getTreasuryP(6).proposal.result, QVAULT_PROPOSAL_INSUFFICIENT_QUORUM);
    EXPECT_EQ(qvault.getTreasuryP(7).proposal.result, QVAULT_PROPOSAL_EXECUTION_FAILED);
    EXPECT_EQ(numberOfPossessedShares(tokenName, tokenIssuer, destination, destination, QX_CONTRACT_INDEX, QX_CONTRACT_INDEX), 300);

    // fund paid the released Qu and two QX transfer fees only
    EXPECT_EQ(state->reinvestingFund, fund - 1000000000ull - 2 * transferFee);

    // passed proposals of an earlier epoch are not run again
    system.epoch++;
    qvault.beginEpoch();
    qvault.endEpoch();
    EXPECT_EQ(getBalance(destination), 1000000000);
    EXPECT_EQ(state->reinvestingFund, fund - 1000000000ull - 2 * transferFee);
}

// QVAULT plus TESTEXA, a test contract that runs shareholder polls
class ContractTestingQvaultPoll : public ContractTestingQvault
{
public:
    ContractTestingQvaultPoll()
    {
        INIT_CONTRACT(TESTEXA);
        callSystemProcedure(TESTEXA_CONTRACT_INDEX, INITIALIZE);
    }

    uint32 submitPollP(const id& address, uint64 shareAssetName, uint32 contractIndex, uint32 pollTick, uint16 pollIndex, uint16 pollType, uint8 numberOfOptions)
    {
        QVAULT::submitPollP_input input;
        QVAULT::submitPollP_output output;
        setMemory(input, 0);
        input.shareAssetName = shareAssetName;
        input.contractIndex = contractIndex;
        input.pollTick = pollTick;
        input.pollIndex = pollIndex;
        input.pollType = pollType;
        input.numberOfOptions = numberOfOptions;
        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 15, input, output, address, QVAULT_PROPOSAL_FEE);
        return output.returnCode;
    }

    QVAULT::voteInPollP_output voteInPollP(const id& address, uint32 proposalId, uint8 option)
    {
        QVAULT::voteInPollP_input input{ proposalId, option };
        QVAULT::voteInPollP_output output;
        setMemory(output, 0);
        invokeUserProcedure(QVAULT_CONTRACT_INDEX, 16, input, output, address, 0);
        return output;
    }

    QVAULT::getPollP_output getPollP(uint32 proposalId) const
    {
        QVAULT::getPollP_input input{ proposalId };
        QVAULT::getPollP_output output;
        callFunction(QVAULT_CONTRACT_INDEX, 24, input, output);
        return output;
    }

    QVAULT::getVoteInProposal_output getVoteInProposal(const id& user, uint64 proposalType, uint64 proposalId) const
    {
        QVAULT::getVoteInProposal_input input{ user, proposalType, proposalId };
        QVAULT::getVoteInProposal_output output;
        callFunction(QVAULT_CONTRACT_INDEX, 21, input, output);
        return output;
    }

    // yes/no poll in TESTEXA about setting its variable 3 to 100
    uint16 openTestExAPoll(const id& proposer)
    {
        TESTEXA::SetShareholderProposal_input input;
        setMemory(input, 0);
        input.proposalData.epoch = system.epoch;
        input.proposalData.type = ProposalTypes::VariableYesNo;
        input.proposalData.data.variableOptions.variable = 2;
        input.proposalData.data.variableOptions.value = 100;
        TESTEXA::SetShareholderProposal_output output = INVALID_PROPOSAL_INDEX;
        EXPECT_TRUE(invokeUserProcedure(TESTEXA_CONTRACT_INDEX, 65534, input, output, proposer, 0));
        return output;
    }

    TESTEXA::GetShareholderProposal_output getTestExAPoll(uint16 pollIndex) const
    {
        TESTEXA::GetShareholderProposal_input input{ pollIndex };
        TESTEXA::GetShareholderProposal_output output;
        callFunction(TESTEXA_CONTRACT_INDEX, 65533, input, output);
        return output;
    }

    // votes QVAULT cast in the TESTEXA poll for the given value
    uint32 qvaultVotesInTestExAPoll(uint16 pollIndex, sint64 value) const
    {
        TESTEXA::GetShareholderVotes_input input{ QVAULT_CONTRACT_ID, pollIndex };
        TESTEXA::GetShareholderVotes_output output;
        callFunction(TESTEXA_CONTRACT_INDEX, 65534, input, output);
        uint32 count = 0;
        for (int i = 0; i < output.voteCounts.capacity(); ++i)
        {
            if (output.voteValues.get(i) == value)
                count += output.voteCounts.get(i);
        }
        return count;
    }
};

TEST(TestContractQvault, pollProposalCastsSharesLive)
{
    system.epoch = 200;
    ContractTestingQvaultPoll qvault;

    const uint64 qcapAssetName = assetNameFromString("QCAP");
    increaseEnergy(QVAULT_QCAP_ISSUER, QVAULT_ISSUE_ASSET_FEE);
    EXPECT_EQ(qvault.issueAsset(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_QCAP_MAX_SUPPLY, 0, 0), QVAULT_QCAP_MAX_SUPPLY);
    increaseEnergy(QVAULT_QCAP_ISSUER, QVAULT_TOKEN_TRANSFER_FEE);
    EXPECT_EQ(qvault.TransferShareOwnershipAndPossession(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_QCAP_MAX_SUPPLY, QVAULT_QCAP_ISSUER, QVAULT_CONTRACT_ID), QVAULT_QCAP_MAX_SUPPLY);

    // stakers A (12000) and B (8000); creating a proposal needs 10000
    const id stakerA = getUser(1);
    const id stakerB = getUser(2);
    const id stakers[2] = { stakerA, stakerB };
    const uint32 amounts[2] = { 12000, 8000 };
    for (int i = 0; i < 2; i++)
    {
        increaseEnergy(QVAULT_CONTRACT_ID, QVAULT_TOKEN_TRANSFER_FEE);
        EXPECT_EQ(qvault.TransferShareOwnershipAndPossession(QVAULT_QCAP_ISSUER, qcapAssetName, amounts[i], QVAULT_CONTRACT_ID, stakers[i]), amounts[i]);
        increaseEnergy(stakers[i], QVAULT_UNSTAKE_FEE);
        EXPECT_EQ(qvault.QXTransferShareManagementRights(QVAULT_QCAP_ISSUER, qcapAssetName, QVAULT_CONTRACT_INDEX, amounts[i], stakers[i]), amounts[i]);
        EXPECT_EQ(qvault.stake(stakers[i], amounts[i]), QVAULT_SUCCESS);
    }
    qvault.endEpoch();
    system.epoch++;

    // QVAULT owns 100 of the 676 TESTEXA shares; a shareholder opens a yes/no poll
    const id shareholder = getUser(10);
    increaseEnergy(shareholder, 1000000);
    std::vector<std::pair<m256i, unsigned int>> shares{ { shareholder, 576 }, { QVAULT_CONTRACT_ID, 100 } };
    issueContractShares(TESTEXA_CONTRACT_INDEX, shares);
    const uint64 testExAName = assetNameFromString("TESTEXA");
    const uint16 pollIndex = qvault.openTestExAPoll(shareholder);
    ASSERT_NE((int)pollIndex, (int)INVALID_PROPOSAL_INDEX);
    const auto poll = qvault.getTestExAPoll(pollIndex).proposal;
    ASSERT_EQ((int)poll.type, (int)ProposalTypes::VariableYesNo);

    // create the matching QVAULT poll proposal
    increaseEnergy(stakerA, QVAULT_PROPOSAL_FEE * 5);
    EXPECT_EQ(qvault.submitPollP(stakerA, assetNameFromString("NOPE"), TESTEXA_CONTRACT_INDEX, poll.tick, pollIndex, poll.type, 2), QVAULT_INSUFFICIENT_SHARE); // QVAULT holds no such shares
    EXPECT_EQ(qvault.submitPollP(stakerA, testExAName, TESTEXA_CONTRACT_INDEX, poll.tick, pollIndex, poll.type, 1), QVAULT_INPUT_ERROR);                       // too few options
    EXPECT_EQ(qvault.submitPollP(stakerA, testExAName, TESTEXA_CONTRACT_INDEX, poll.tick, pollIndex, poll.type, 2), QVAULT_SUCCESS);
    EXPECT_EQ(qvault.submitPollP(stakerA, testExAName, TESTEXA_CONTRACT_INDEX, poll.tick, pollIndex, poll.type, 2), QVAULT_INPUT_ERROR);                       // duplicate
    EXPECT_EQ(qvault.getPollP(0).numberOfPollP, 1u);
    EXPECT_EQ((int)qvault.getPollP(0).proposal.castOption, -1);
    EXPECT_EQ(qvault.qvaultVotesInTestExAPoll(pollIndex, 0) + qvault.qvaultVotesInTestExAPoll(pollIndex, 1), 0u);

    // B votes yes: QVAULT casts all 100 shares for yes
    auto voted = qvault.voteInPollP(stakerB, 0, 1);
    EXPECT_EQ(voted.returnCode, QVAULT_SUCCESS);
    EXPECT_EQ((int)voted.castOption, 1);
    EXPECT_EQ(qvault.qvaultVotesInTestExAPoll(pollIndex, 1), 100u);

    // A (more voting power) votes no: QVAULT switches to no
    voted = qvault.voteInPollP(stakerA, 0, 0);
    EXPECT_EQ((int)voted.castOption, 0);
    EXPECT_EQ(qvault.qvaultVotesInTestExAPoll(pollIndex, 0), 100u);
    EXPECT_EQ(qvault.qvaultVotesInTestExAPoll(pollIndex, 1), 0u);
    EXPECT_EQ(qvault.getPollP(0).proposal.optionVotes.get(0), 12000u);
    EXPECT_EQ(qvault.getPollP(0).proposal.optionVotes.get(1), 8000u);

    // A changes to yes: QVAULT back to yes, A's power moves
    voted = qvault.voteInPollP(stakerA, 0, 1);
    EXPECT_EQ((int)voted.castOption, 1);
    EXPECT_EQ(qvault.qvaultVotesInTestExAPoll(pollIndex, 1), 100u);
    EXPECT_EQ(qvault.getPollP(0).proposal.optionVotes.get(0), 0u);
    EXPECT_EQ(qvault.getPollP(0).proposal.optionVotes.get(1), 20000u);
    EXPECT_EQ(qvault.getPollP(0).proposal.totalVotes, 20000u);
    EXPECT_EQ(qvault.getVoteInProposal(stakerA, QVAULT_PROPOSAL_TYPE_POLL, 0).votingDecision, 1);

    // refused votes
    EXPECT_EQ(qvault.voteInPollP(stakerA, 0, 1).returnCode, QVAULT_SAME_DECISION);
    EXPECT_EQ(qvault.voteInPollP(stakerB, 0, 2).returnCode, QVAULT_INPUT_ERROR);       // only options 0 and 1
    EXPECT_EQ(qvault.voteInPollP(getUser(3), 0, 1).returnCode, QVAULT_NO_VOTING_POWER);
    EXPECT_EQ(qvault.voteInPollP(stakerA, 1, 1).returnCode, QVAULT_OVERFLOW_PROPOSAL);

    // end of epoch records the outcome
    qvault.endEpoch();
    EXPECT_EQ(qvault.getPollP(0).proposal.result, QVAULT_POLL_VOTED);
    EXPECT_EQ((int)qvault.getPollP(0).proposal.castOption, 1);

    // closed in later epochs
    system.epoch++;
    EXPECT_EQ(qvault.voteInPollP(stakerB, 0, 0).returnCode, QVAULT_ENDED_PROPOSAL);
}
