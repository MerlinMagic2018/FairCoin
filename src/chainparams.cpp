// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include "chainparamsseeds.h"
#include "pow.h"
#include "arith_uint256.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 521142271 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the Yespower genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
CBlock(hash=0a1f39a5629a83e3d194c3cfb6876fa09011392772db1a14d45c577d43a24a1d, yespowerpow=0002f9944489c8fc96619a1c4473f8b08a804882fe2b641a6e5f916a06b63f63, ver=0x00000001, 
hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, 
hashMerkleRoot=b878d2ba72742d90114db17c9708956b9f72c5e668515b103dbdbfa3f94ec683, nTime=1721228774, nBits=1f0fffff, nNonce=638, vtx=1)
  CTransaction(hash=b878d2ba72, ver=1, vin.size=1, vout.size=1, nLockTime=0)
    CTxIn(COutPoint(0000000000, 4294967295), coinbase 04ffff0f1f01040e4a756c792f313774682f32303234)
    CScriptWitness()
    CTxOut(nValue=50.00000000, scriptPubKey=4104c87d66acdcda4765751a478397)
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "July/17th/2024";
    const CScript genesisOutputScript = CScript() << ParseHex("04c87d66acdcda4765751a47839702ec12245ace9245ec55e7b6bcaab4cc2144aec1c4e3db680b7b4c6ed6ac4aaea18c3999fafaba3443afb8b5883f57f634b97e") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash   = uint256S("0x0a1f39a5629a83e3d194c3cfb6876fa09011392772db1a14d45c577d43a24a1d");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit    = uint256S("0x000fffff00000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60;
        consensus.nPowTargetSpacing  = 2.5 * 60;           
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512;  // 75% of 20160
        consensus.nMinerConfirmationWindow = 2016;    // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1718046043; // June 10th, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1749582043; // June 10th, 2025

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1718046043; // June 10th, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1749582043; // June 10th, 2025

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =  uint256S ("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S ("0x0a1f39a5629a83e3d194c3cfb6876fa09011392772db1a14d45c577d43a24a1d"); // 0

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x67;
        pchMessageStart[3] = 0x34;
        pchMessageStart[1] = 0x10;
        pchMessageStart[2] = 0x85;
        nDefaultPort       = 9876;
        nPruneAfterHeight  = 1000000;

        genesis = CreateGenesisBlock(1721228774, 638, 0x1f0fffff, 1, 50 * COIN);

    /*    uint32_t nTime = 1721228774;
        uint32_t nNonce = 0;

        if (nNonce == 0) {
           while (UintToArith256(genesis.GetHashYespower()) > UintToArith256(consensus.powLimit)) {
              nNonce++;
              genesis = CreateGenesisBlock(nTime, nNonce, 0x1f0fffff, 1, 50 * COIN);
              if (nNonce % 128 == 0) printf("\rnonce %08x", nNonce);
           }
           printf("\n%s\n", genesis.ToString().c_str());
        }
        genesis = CreateGenesisBlock(nTime, nNonce, 0x1f0fffff, 1, 50 * COIN);*/

        consensus.hashGenesisBlock = genesis.GetHash();

        assert (consensus.hashGenesisBlock == uint256S("0x0a1f39a5629a83e3d194c3cfb6876fa09011392772db1a14d45c577d43a24a1d"));
        assert (genesis.hashMerkleRoot == uint256S("0xb878d2ba72742d90114db17c9708956b9f72c5e668515b103dbdbfa3f94ec683"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,36);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,63);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
        vSeeds.clear();
    //    vSeeds.emplace_back ("node.faircoin.com", false);

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
                { 0,     uint256S("0x0a1f39a5629a83e3d194c3cfb6876fa09011392772db1a14d45c577d43a24a1d") }
            }
        };

        chainTxData = ChainTxData {
            // Data as of genesis block.
            1721228774,  // * UNIX timestamp of last known number of transactions
            0,        // * total number of transactions between genesis and that timestamp
                         //   (the tx=... number in the SetBestChain debug.log lines)
            0.0 // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v1)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 840000;

        consensus.BIP34Height = 0;
        consensus.BIP34Hash   = uint256S("0x0a1f39a5629a83e3d194c3cfb6876fa09011392772db1a14d45c577d43a24a1d");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit    = uint256S("0x000fffff00000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60;
        consensus.nPowTargetSpacing  = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512;  // 75% of 20160
        consensus.nMinerConfirmationWindow = 2016;    // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1718046043; // June 10th, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1749582043; // June 10th, 2025

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1718046043; // June 10th, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1749582043; // June 10th, 2025

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =  uint256S ("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S ("0x0a1f39a5629a83e3d194c3cfb6876fa09011392772db1a14d45c577d43a24a1d"); // 0

        pchMessageStart[0] = 0x58;
        pchMessageStart[1] = 0x38;
        pchMessageStart[2] = 0x62;
        pchMessageStart[3] = 0x41;
        nDefaultPort       = 19876;
        nPruneAfterHeight  = 1000;

        genesis = CreateGenesisBlock(1721228774, 638, 0x1f0fffff, 1, 50 * COIN);

  /*      uint32_t nTime = 1721228774;
        uint32_t nNonce = 0;

        if (nNonce == 0) {
           while (UintToArith256(genesis.GetHashYespower()) > UintToArith256(consensus.powLimit)) {
              nNonce++;
              genesis = CreateGenesisBlock(nTime, nNonce, 0x1f0fffff, 1, 50 * COIN);
              if (nNonce % 128 == 0) printf("\rnonce %08x", nNonce);
           }
           printf("\n%s\n", genesis.ToString().c_str());
        }
        genesis = CreateGenesisBlock(nTime, nNonce, 0x1f0fffff, 1, 50 * COIN);*/

        consensus.hashGenesisBlock = genesis.GetHash();

        assert (consensus.hashGenesisBlock == uint256S("0x0a1f39a5629a83e3d194c3cfb6876fa09011392772db1a14d45c577d43a24a1d"));
        assert (genesis.hashMerkleRoot == uint256S("0xb878d2ba72742d90114db17c9708956b9f72c5e668515b103dbdbfa3f94ec683"));

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));
   //     vSeeds.emplace_back ("node.faircoin.com", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
		{0, uint256S("0x0a1f39a5629a83e3d194c3cfb6876fa09011392772db1a14d45c577d43a24a1d")}
            }
        };

        chainTxData = ChainTxData {
            /* data as of:  */
            1721228774,
            0,
            0.0
        };
    }
};

/**
 * Regression test
CBlock(hash=409a4be16fb9084a53c45e2ca0edd76c65a269bc1312be47cebef23dc3ce552f, yespowerpow=0cdd08b95f3ebe2e5c0d77e41c794126237819a518097df7f67fc9c12ecf5a1f, ver=0x00000001, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=b878d2ba72742d90114db17c9708956b9f72c5e668515b103dbdbfa3f94ec683, nTime=1721228774, nBits=200fffff, nNonce=14, vtx=1)
  CTransaction(hash=b878d2ba72, ver=1, vin.size=1, vout.size=1, nLockTime=0)
    CTxIn(COutPoint(0000000000, 4294967295), coinbase 04ffff0f1f01040e4a756c792f313774682f32303234)
    CScriptWitness()
    CTxOut(nValue=50.00000000, scriptPubKey=4104c87d66acdcda4765751a478397)
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 35;

        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 0; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("0fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60;
        consensus.nPowTargetSpacing  = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork  = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x47;
        pchMessageStart[1] = 0x28;
        pchMessageStart[2] = 0x74;
        pchMessageStart[3] = 0x32;
        nDefaultPort       = 19444;
        nPruneAfterHeight  = 1000;

        genesis = CreateGenesisBlock(1721228774, 14, 0x200fffff, 1, 50 * COIN);

   /*     uint32_t nTime = 1721228774;
        uint32_t nNonce = 0;

        if (nNonce == 0) {
           while (UintToArith256(genesis.GetHashYespower()) > UintToArith256(consensus.powLimit)) {
              nNonce++;
              genesis = CreateGenesisBlock(nTime, nNonce, 0x200fffff, 1, 50 * COIN);
              if (nNonce % 128 == 0) printf("\rnonce %08x", nNonce);
           }
           printf("\n%s\n", genesis.ToString().c_str());
        }
        genesis = CreateGenesisBlock(nTime, nNonce, 0x200fffff, 1, 50 * COIN);*/

        consensus.hashGenesisBlock = genesis.GetHash();

        assert (consensus.hashGenesisBlock == uint256S("0x409a4be16fb9084a53c45e2ca0edd76c65a269bc1312be47cebef23dc3ce552f"));
        assert (genesis.hashMerkleRoot == uint256S("0xb878d2ba72742d90114db17c9708956b9f72c5e668515b103dbdbfa3f94ec683"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fMiningRequiresPeers = false;

        checkpointData = (CCheckpointData) {
            {
                // {0, uint256S("00")},
            }
        };

        chainTxData = ChainTxData {
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
