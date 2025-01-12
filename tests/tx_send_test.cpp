/*!
 * minter_tx.
 * tx_send_test.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <gtest/gtest.h>

#include "minter/tx/tx.h"
#include "minter/tx/tx_builder.h"
#include "minter/private_key.h"
#include "minter/tx/tx_send_coin.h"

TEST(TxSend, TestSendEncode) {
    const char* expectedTx = "f8840102018a4d4e540000000000000001aae98a4d4e5400000000000000940000000000000000000000000000000000000000888ac7230489e80000808001b845f8431ba07a27eed01eae753c028d74e189f23b371fdd936dc0274d36ab27d8f0f4b2e9e1a03cd85dcf65e4204b393de0cd4db03d47479ba4321f074bedff2049b6e048fb6f";

    auto tx_builder = minter::new_tx();
    tx_builder->set_nonce("1");
    tx_builder->set_gas_price("1");
    tx_builder->set_gas_coin("MNT");
    tx_builder->set_chain_id(minter::testnet);
    auto data = tx_builder->tx_send_coin();
    data->set_to("Mx0000000000000000000000000000000000000000");
    data->set_value("10");
    data->set_coin("MNT");
    auto tx = data->build();

    minter::privkey_t pk("df1f236d0396cc43147e44206c341a65573326e907d033690e31a21323c03a9f");
    auto signedTx = tx->sign_single(pk);

    {
        auto decoded = minter::tx::decode(signedTx.toHex().c_str());
        ASSERT_STREQ("MNT", decoded->get_gas_coin().c_str());
        ASSERT_EQ(dev::bigint("1"), decoded->get_gas_price());
        ASSERT_EQ(minter::testnet, decoded->get_chain_id());
        ASSERT_EQ(dev::bigint("1"), decoded->get_nonce());
        ASSERT_EQ((uint8_t)0x01, decoded->get_type());
        ASSERT_EQ(dev::bytes(0), decoded->get_service_data());
        ASSERT_EQ(dev::bytes(0), decoded->get_payload());
        ASSERT_EQ(minter::signature_type::single, decoded->get_signature_type());
        std::shared_ptr<minter::tx_send_coin> data = decoded->get_data<minter::tx_send_coin>();
        ASSERT_EQ(minter::address_t("Mx0000000000000000000000000000000000000000"), data->get_to());
        ASSERT_EQ(dev::bigdec18("10"), data->get_value());
        ASSERT_STREQ("MNT", data->get_coin().c_str());
    }

    ASSERT_STREQ(expectedTx, signedTx.toHex().c_str());
}

TEST(TxSend, TestSendDecode) {
    const char* encoded = "f8840102018a4d4e540000000000000001aae98a4d4e5400000000000000940000000000000000000000000000000000000000888ac7230489e80000808001b845f8431ba07a27eed01eae753c028d74e189f23b371fdd936dc0274d36ab27d8f0f4b2e9e1a03cd85dcf65e4204b393de0cd4db03d47479ba4321f074bedff2049b6e048fb6f";

    std::shared_ptr<minter::tx> decoded = minter::tx::decode(encoded);
    minter::address_t exAddress = "Mx0000000000000000000000000000000000000000";

    ASSERT_EQ(dev::bigint("1"), decoded->get_nonce());
    ASSERT_STREQ("MNT", decoded->get_gas_coin().c_str());
    ASSERT_EQ(dev::bigint("1"), decoded->get_gas_price());
    ASSERT_NE(dev::bigint("2"), decoded->get_gas_price());
    ASSERT_EQ(minter::testnet, decoded->get_chain_id());
    std::shared_ptr<minter::tx_send_coin> data = decoded->get_data<minter::tx_send_coin>();
    ASSERT_EQ(exAddress, data->get_to());
    ASSERT_EQ(dev::bigdec18("10"), data->get_value());
    ASSERT_STREQ("MNT", data->get_coin().c_str());
}

TEST(TxSend, TestEncodeForAndroidSDK) {
    auto tx_builder = minter::new_tx();
    tx_builder->set_nonce("128");
    tx_builder->set_gas_price("1");
    tx_builder->set_gas_coin("MNT");
    tx_builder->set_chain_id(minter::testnet);
    auto data = tx_builder->tx_send_coin();
    data->set_to("Mxb445feaf3eb747ac52426b054aa42b0b3d913e1f");
    data->set_value("1");
    data->set_coin("MNT");
    auto tx = data->build();

    minter::privkey_t pk = minter::privkey_t::from_mnemonic("body attitude enable enjoy swift wise example hammer trap saddle bike lobster");
    auto signedTx = tx->sign_single(pk);

    {
        auto decoded = minter::tx::decode(signedTx.toHex().c_str());
        ASSERT_STREQ("MNT", decoded->get_gas_coin().c_str());
        ASSERT_EQ(dev::bigint("1"), decoded->get_gas_price());
        ASSERT_EQ(minter::testnet, decoded->get_chain_id());
        ASSERT_EQ(dev::bigint("128"), decoded->get_nonce());
        ASSERT_EQ((uint8_t)0x01, decoded->get_type());
        ASSERT_EQ(dev::bytes(0), decoded->get_service_data());
        ASSERT_EQ(dev::bytes(0), decoded->get_payload());
        ASSERT_EQ(minter::signature_type::single, decoded->get_signature_type());
        auto data = decoded->get_data<minter::tx_send_coin>();
        ASSERT_EQ(minter::address_t("Mxb445feaf3eb747ac52426b054aa42b0b3d913e1f"), data->get_to());
        ASSERT_EQ(dev::bigdec18("1"), data->get_value());
        ASSERT_STREQ("MNT", data->get_coin().c_str());
    }

    std::cout << signedTx.toHex() << '\n';
}