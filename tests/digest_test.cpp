/*******************************************************************************
 * tests/digest_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/digest/md5.hpp>
#include <tlx/digest/sha1.hpp>
#include <tlx/digest/sha256.hpp>
#include <tlx/digest/sha512.hpp>

#include <tlx/die.hpp>
#include <tlx/string/hexdump.hpp>

using namespace tlx;

void test_sha256() {
    die_unequal(
        sha256_hex_uc(""),
        "E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855");

    die_unequal(
        sha256_hex_uc("abc"),
        "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD");

    die_unequal(
        sha256_hex_uc(
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"),
        "248D6A61D20638B8E5C026930C3E6039A33CE45964FF2167F6ECEDD419DB06C1");

    die_unequal(
        sha256_hex_uc(
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
            "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"),
        "CF5B16A778AF8380036CE59E7B0492370B249B11E8F07A51AFAC45037AFEE9D1");

    die_unequal(
        sha256_hex_uc(std::string(1000000, 'a')),
        "CDC76E5C9914FB9281A1C7E284D73E67F1809A48A497200E046D39CCC7112CD0");

    std::vector<std::pair<const char*, const char*> > test_vectors = {
        { "d3",
          "28969cdfa74a12c82f3bad960b0b000aca2ac329deea5c2328ebc6f2ba9802c1" },
        { "11af",
          "5ca7133fa735326081558ac312c620eeca9970d1e70a4b95533d956f072d1f98" },
        { "b4190e",
          "dff2e73091f6c05e528896c4c831b9448653dc2ff043528f6769437bc7b975c2" },
        { "74ba2521",
          "b16aa56be3880d18cd41e68384cf1ec8c17680c45a02b1575dc1518923ae8b0e" },
        { "c299209682",
          "f0887fe961c9cd3beab957e8222494abb969b1ce4c6557976df8b0f6d20e9166" },
        { "e1dc724d5621",
          "eca0a060b489636225b4fa64d267dabbe44273067ac679f20820bddc6b6a90ac" },
        { "06e076f5a442d5",
          "3fd877e27450e6bbd5d74bb82f9870c64c66e109418baa8e6bbcff355e287926" },
        { "5738c929c4f4ccb6",
          "963bb88f27f512777aab6c8b1a02c70ec0ad651d428f870036e1917120fb48bf" },
        { "3334c58075d3f4139e",
          "078da3d77ed43bd3037a433fd0341855023793f9afd08b4b08ea1e5597ceef20" },
        { "74cb9381d89f5aa73368",
          "73d6fad1caaa75b43b21733561fd3958bdc555194a037c2addec19dc2d7a52bd" },
        { "76ed24a0f40a41221ebfcf",
          "044cef802901932e46dc46b2545e6c99c0fc323a0ed99b081bda4216857f38ac" },
        { "9baf69cba317f422fe26a9a0",
          "fe56287cd657e4afc50dba7a3a54c2a6324b886becdcd1fae473b769e551a09b" },
        { "68511cdb2dbbf3530d7fb61cbc",
          "af53430466715e99a602fc9f5945719b04dd24267e6a98471f7a7869bd3b4313" },
        { "af397a8b8dd73ab702ce8e53aa9f",
          "d189498a3463b18e846b8ab1b41583b0b7efc789dad8a7fb885bbf8fb5b45c5c" },
        { "294af4802e5e925eb1c6cc9c724f09",
          "dcbaf335360de853b9cddfdafb90fa75567d0d3d58af8db9d764113aef570125" },
        { "0a27847cdc98bd6f62220b046edd762b",
          "80c25ec1600587e7f28b18b1b18e3cdc89928e39cab3bc25e4d4a4c139bcedc4" },
        { "1b503fb9a73b16ada3fcf1042623ae7610",
          "d5c30315f72ed05fe519a1bf75ab5fd0ffec5ac1acb0daf66b6b769598594509" },
        { "59eb45bbbeb054b0b97334d53580ce03f699",
          "32c38c54189f2357e96bd77eb00c2b9c341ebebacc2945f97804f59a93238288" },
        { "58e5a3259cb0b6d12c83f723379e35fd298b60",
          "9b5b37816de8fcdf3ec10b745428708df8f391c550ea6746b2cafe019c2b6ace" },
        { "c1ef39cee58e78f6fcdc12e058b7f902acd1a93b",
          "6dd52b0d8b48cc8146cebd0216fbf5f6ef7eeafc0ff2ff9d1422d6345555a142" },
        { "9cab7d7dcaec98cb3ac6c64dd5d4470d0b103a810c",
          "44d34809fc60d1fcafa7f37b794d1d3a765dd0d23194ebbe340f013f0c39b613" },
        { "ea157c02ebaf1b22de221b53f2353936d2359d1e1c97",
          "9df5c16a3f580406f07d96149303d8c408869b32053b726cf3defd241e484957" },
        { "da999bc1f9c7acff32828a73e672d0a492f6ee895c6867",
          "672b54e43f41ee77584bdf8bf854d97b6252c918f7ea2d26bc4097ea53a88f10" },
        { "47991301156d1d977c0338efbcad41004133aefbca6bcf7e",
          "feeb4b2b59fec8fdb1e55194a493d8c871757b5723675e93d3ac034b380b7fc9" },
        { "2e7ea84da4bc4d7cfb463e3f2c8647057afff3fbececa1d200",
          "76e3acbc718836f2df8ad2d0d2d76f0cfa5fea0986be918f10bcee730df441b9" },
        { "47c770eb4549b6eff6381d62e9beb464cd98d341cc1c09981a7a",
          "6733809c73e53666c735b3bd3daf87ebc77c72756150a616a194108d71231272" },
        { "ac4c26d8b43b8579d8f61c9807026e83e9b586e1159bd43b851937",
          "0e6e3c143c3a5f7f38505ed6adc9b48c18edf6dedf11635f6e8f9ac73c39fe9e" },
        { "0777fc1e1ca47304c2e265692838109e26aab9e5c4ae4e8600df4b1f",
          "ffb4fc03e054f8ecbc31470fc023bedcd4a406b9dd56c71da1b660dcc4842c65" },
        { "1a57251c431d4e6c2e06d65246a296915071a531425ecf255989422a66",
          "c644612cd326b38b1c6813b1daded34448805aef317c35f548dfb4a0d74b8106" },
        { "9b245fdad9baeb890d9c0d0eff816efb4ca138610bc7d78cb1a801ed3273",
          "c0e29eeeb0d3a7707947e623cdc7d1899adc70dd7861205ea5e5813954fb7957" },
        { "95a765809caf30ada90ad6d61c2b4b30250df0a7ce23b7753c9187f4319ce2",
          "a4139b74b102cf1e2fce229a6cd84c87501f50afa4c80feacf7d8cf5ed94f042" },
        { "09fc1accc230a205e4a208e64a8f204291f581a12756392da4b8c0cf5ef02b95",
          "4f44c1c7fbebb6f9601829f3897bfd650c56fa07844be76489076356ac1886a4" },
        { "0546f7b8682b5b95fd32385faf25854cb3f7b40cc8fa229fbd52b16934aab388"
          "a7",
          "b31ad3cd02b10db282b3576c059b746fb24ca6f09fef69402dc90ece7421cbb7" },
        { "b12db4a1025529b3b7b1e45c6dbc7baa8897a0576e66f64bf3f8236113a6276e"
          "e77d",
          "1c38bf6bbfd32292d67d1d651fd9d5b623b6ec1e854406223f51d0df46968712" },
        { "e68cb6d8c1866c0a71e7313f83dc11a5809cf5cfbeed1a587ce9c2c92e022abc"
          "1644bb",
          "c2684c0dbb85c232b6da4fb5147dd0624429ec7e657991edd95eda37a587269e" },
        { "4e3d8ac36d61d9e51480831155b253b37969fe7ef49db3b39926f3a00b69a367"
          "74366000",
          "bf9d5e5b5393053f055b380baed7e792ae85ad37c0ada5fd4519542ccc461cf3" },
        { "03b264be51e4b941864f9b70b4c958f5355aac294b4b87cb037f11f85f07eb57"
          "b3f0b89550",
          "d1f8bd684001ac5a4b67bbf79f87de524d2da99ac014dec3e4187728f4557471" },
        { "d0fefd96787c65ffa7f910d6d0ada63d64d5c4679960e7f06aeb8c70dfef954f"
          "8e39efdb629b",
          "49ba38db85c2796f85ffd57dd5ec337007414528ae33935b102d16a6b91ba6c1" },
        { "b7c79d7e5f1eeccdfedf0e7bf43e730d447e607d8d1489823d09e11201a0b125"
          "8039e7bd4875b1",
          "725e6f8d888ebaf908b7692259ab8839c3248edd22ca115bb13e025808654700" },
        { "64cd363ecce05fdfda2486d011a3db95b5206a19d3054046819dd0d36783955d"
          "7e5bf8ba18bf738a",
          "32caef024f84e97c30b4a7b9d04b678b3d8a6eb2259dff5b7f7c011f090845f8" },
        { "6ac6c63d618eaf00d91c5e2807e83c093912b8e202f78e139703498a79c6067f"
          "54497c6127a23910a6",
          "4bb33e7c6916e08a9b3ed6bcef790aaaee0dcf2e7a01afb056182dea2dad7d63" },
        { "d26826db9baeaa892691b68900b96163208e806a1da077429e454fa011840951"
          "a031327e605ab82ecce2",
          "3ac7ac6bed82fdc8cd15b746f0ee7489158192c238f371c1883c9fe90b3e2831" },
        { "3f7a059b65d6cb0249204aac10b9f1a4ac9e5868adebbe935a9eb5b9019e1c93"
          "8bfc4e5c5378997a3947f2",
          "bfce809534eefe871273964d32f091fe756c71a7f512ef5f2300bcd57f699e74" },
        { "60ffcb23d6b88e485b920af81d1083f6291d06ac8ca3a965b85914bc2add4054"
          "4a027fca936bbde8f359051c",
          "1d26f3e04f89b4eaa9dbed9231bb051eef2e8311ad26fe53d0bf0b821eaf7567" },
        { "9ecd07b684bb9e0e6692e320cec4510ca79fcdb3a2212c26d90df65db33e692d"
          "073cc174840db797504e482eef",
          "0ffeb644a49e787ccc6970fe29705a4f4c2bfcfe7d19741c158333ff6982cc9c" },
        { "9d64de7161895884e7fa3d6e9eb996e7ebe511b01fe19cd4a6b3322e80aaf52b"
          "f6447ed1854e71001f4d54f8931d",
          "d048ee1524014adf9a56e60a388277de194c694cc787fc5a1b554ea9f07abfdf" },
        { "c4ad3c5e78d917ecb0cbbcd1c481fc2aaf232f7e289779f40e504cc309662ee9"
          "6fecbd20647ef00e46199fbc482f46",
          "50dbf40066f8d270484ee2ef6632282dfa300a85a8530eceeb0e04275e1c1efd" },
        { "4eef5107459bddf8f24fc7656fd4896da8711db50400c0164847f692b886ce8d"
          "7f4d67395090b3534efd7b0d298da34b",
          "7c5d14ed83dab875ac25ce7feed6ef837d58e79dc601fb3c1fca48d4464e8b83" },
        { "047d2758e7c2c9623f9bdb93b6597c5e84a0cd34e610014bcb25b49ed05c7e35"
          "6e98c7a672c3dddcaeb84317ef614d342f",
          "7d53eccd03da37bf58c1962a8f0f708a5c5c447f6a7e9e26137c169d5bdd82e4" },
        { "3d83df37172c81afd0de115139fbf4390c22e098c5af4c5ab4852406510bc0e6"
          "cf741769f44430c5270fdae0cb849d71cbab",
          "99dc772e91ea02d9e421d552d61901016b9fd4ad2df4a8212c1ec5ba13893ab2" },
        { "33fd9bc17e2b271fa04c6b93c0bdeae98654a7682d31d9b4dab7e6f32cd58f2f"
          "148a68fbe7a88c5ab1d88edccddeb30ab21e5e",
          "cefdae1a3d75e792e8698d5e71f177cc761314e9ad5df9602c6e60ae65c4c267" },
        { "77a879cfa11d7fcac7a8282cc38a43dcf37643cc909837213bd6fd95d956b219"
          "a1406cbe73c52cd56c600e55b75bc37ea69641bc",
          "c99d64fa4dadd4bc8a389531c68b4590c6df0b9099c4d583bc00889fb7b98008" },
        { "45a3e6b86527f20b4537f5af96cfc5ad8777a2dde6cf7511886c5590ece24fc6"
          "1b226739d207dabfe32ba6efd9ff4cd5db1bd5ead3",
          "4d12a849047c6acd4b2eee6be35fa9051b02d21d50d419543008c1d82c427072" },
        { "25362a4b9d74bde6128c4fdc672305900947bc3ada9d9d316ebcf1667ad43631"
          "89937251f149c72e064a48608d940b7574b17fefc0df",
          "f8e4ccab6c979229f6066cc0cb0cfa81bb21447c16c68773be7e558e9f9d798d" },
        { "3ebfb06db8c38d5ba037f1363e118550aad94606e26835a01af05078533cc25f"
          "2f39573c04b632f62f68c294ab31f2a3e2a1a0d8c2be51",
          "6595a2ef537a69ba8583dfbf7f5bec0ab1f93ce4c8ee1916eff44a93af5749c4" },
        { "2d52447d1244d2ebc28650e7b05654bad35b3a68eedc7f8515306b496d75f3e7"
          "3385dd1b002625024b81a02f2fd6dffb6e6d561cb7d0bd7a",
          "cfb88d6faf2de3a69d36195acec2e255e2af2b7d933997f348e09f6ce5758360" },
        { "4cace422e4a015a75492b3b3bbfbdf3758eaff4fe504b46a26c90dacc119fa90"
          "50f603d2b58b398cad6d6d9fa922a154d9e0bc4389968274b0",
          "4d54b2d284a6794581224e08f675541c8feab6eefa3ac1cfe5da4e03e62f72e4" },
        { "8620b86fbcaace4ff3c2921b8466ddd7bacae07eefef693cf17762dcabb89a84"
          "010fc9a0fb76ce1c26593ad637a61253f224d1b14a05addccabe",
          "dba490256c9720c54c612a5bd1ef573cd51dc12b3e7bd8c6db2eabe0aacb846b" },
        { "d1be3f13febafefc14414d9fb7f693db16dc1ae270c5b647d80da8583587c1ad"
          "8cb8cb01824324411ca5ace3ca22e179a4ff4986f3f21190f3d7f3",
          "02804978eba6e1de65afdbc6a6091ed6b1ecee51e8bff40646a251de6678b7ef" },
        { "f499cc3f6e3cf7c312ffdfba61b1260c37129c1afb391047193367b7b2edeb57"
          "9253e51d62ba6d911e7b818ccae1553f6146ea780f78e2219f629309",
          "0b66c8b4fefebc8dc7da0bbedc1114f228aa63c37d5c30e91ab500f3eadfcec5" },
        { "6dd6efd6f6caa63b729aa8186e308bc1bda06307c05a2c0ae5a3684e6e460811"
          "748690dc2b58775967cfcc645fd82064b1279fdca771803db9dca0ff53",
          "c464a7bf6d180de4f744bb2fe5dc27a3f681334ffd54a9814650e60260a478e3" },
        { "6511a2242ddb273178e19a82c57c85cb05a6887ff2014cf1a31cb9ba5df1695a"
          "adb25c22b3c5ed51c10d047d256b8e3442842ae4e6c525f8d7a5a944af2a",
          "d6859c0b5a0b66376a24f56b2ab104286ed0078634ba19112ace0d6d60a9c1ae" },
        { "e2f76e97606a872e317439f1a03fcd92e632e5bd4e7cbc4e97f1afc19a16fde9"
          "2d77cbe546416b51640cddb92af996534dfd81edb17c4424cf1ac4d75aceeb",
          "18041bd4665083001fba8c5411d2d748e8abbfdcdfd9218cb02b68a78e7d4c23" },
        { "5a86b737eaea8ee976a0a24da63e7ed7eefad18a101c1211e2b3650c5187c2a8"
          "a650547208251f6d4237e661c7bf4c77f335390394c37fa1a9f9be836ac28509",
          "42e61e174fbb3897d6dd6cef3dd2802fe67b331953b06114a65c772859dfc1aa" }
    };

    for (const auto& p : test_vectors) {
        die_unequal(sha256_hex(tlx::parse_hexdump(p.first)), p.second);
    }
}

void test_sha512() {
    die_unequal(
        sha512_hex_uc(""),
        "CF83E1357EEFB8BDF1542850D66D8007D620E4050B5715DC83F4A921D36CE9CE"
        "47D0D13C5D85F2B0FF8318D2877EEC2F63B931BD47417A81A538327AF927DA3E");

    die_unequal(
        sha512_hex_uc("abc"),
        "DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A"
        "2192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F");

    die_unequal(
        sha512_hex_uc(
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"),
        "204A8FC6DDA82F0A0CED7BEB8E08A41657C16EF468B228A8279BE331A703C335"
        "96FD15C13B1B07F9AA1D3BEA57789CA031AD85C7A71DD70354EC631238CA3445");

    die_unequal(
        sha512_hex_uc(
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
            "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"),
        "8E959B75DAE313DA8CF4F72814FC143F8F7779C6EB9F7FA17299AEADB6889018"
        "501D289E4900F7E4331B99DEC4B5433AC7D329EEB6DD26545E96E55B874BE909");

    die_unequal(
        sha512_hex_uc(std::string(1000000, 'a')),
        "E718483D0CE769644E2E42C7BC15B4638E1F98B13B2044285632A803AFA973EB"
        "DE0FF244877EA60A4CB0432CE577C31BEB009C5C2C49AA2E4EADB217AD8CC09B");

    std::vector<std::pair<const char*, const char*> > test_vectors = {
        { "21",
          "3831a6a6155e509dee59a7f451eb35324d8f8f2df6e3708894740f98fdee2388"
          "9f4de5adb0c5010dfb555cda77c8ab5dc902094c52de3278f35a75ebc25f093a" },
        { "9083",
          "55586ebba48768aeb323655ab6f4298fc9f670964fc2e5f2731e34dfa4b0c09e"
          "6e1e12e3d7286b3145c61c2047fb1a2a1297f36da64160b31fa4c8c2cddd2fb4" },
        { "0a55db",
          "7952585e5330cb247d72bae696fc8a6b0f7d0804577e347d99bc1b11e52f3849"
          "85a428449382306a89261ae143c2f3fb613804ab20b42dc097e5bf4a96ef919b" },
        { "23be86d5",
          "76d42c8eadea35a69990c63a762f330614a4699977f058adb988f406fb0be8f2"
          "ea3dce3a2bbd1d827b70b9b299ae6f9e5058ee97b50bd4922d6d37ddc761f8eb" },
        { "eb0ca946c1",
          "d39ecedfe6e705a821aee4f58bfc489c3d9433eb4ac1b03a97e321a2586b40dd"
          "0522f40fa5aef36afff591a78c916bfc6d1ca515c4983dd8695b1ec7951d723e" },
        { "38667f39277b",
          "85708b8ff05d974d6af0801c152b95f5fa5c06af9a35230c5bea2752f031f9bd"
          "84bd844717b3add308a70dc777f90813c20b47b16385664eefc88449f04f2131" },
        { "b39f71aaa8a108",
          "258b8efa05b4a06b1e63c7a3f925c5ef11fa03e3d47d631bf4d474983783d8c0"
          "b09449009e842fc9fa15de586c67cf8955a17d790b20f41dadf67ee8cdcdfce6" },
        { "6f8d58b7cab1888c",
          "a3941def2803c8dfc08f20c06ba7e9a332ae0c67e47ae57365c243ef40059b11"
          "be22c91da6a80c2cff0742a8f4bcd941bdee0b861ec872b215433ce8dcf3c031" },
        { "162b0cf9b3750f9438",
          "ade217305dc34392aa4b8e57f64f5a3afdd27f1fa969a9a2608353f82b95cfb4"
          "ae84598d01575a578a1068a59b34b5045ff6d5299c5cb7ee17180701b2d1d695" },
        { "bad7c618f45be207975e",
          "5886828959d1f82254068be0bd14b6a88f59f534061fb20376a0541052dd3635"
          "edf3c6f0ca3d08775e13525df9333a2113c0b2af76515887529910b6c793c8a5" },
        { "6213e10a4420e0d9b77037",
          "9982dc2a04dff165567f276fd463efef2b369fa2fbca8cee31ce0de8a79a2eb0"
          "b53e437f7d9d1f41c71d725cabb949b513075bad1740c9eefbf6a5c6633400c7" },
        { "6332c3c2a0a625a61df71858",
          "9d60375d9858d9f2416fb86fa0a2189ee4213e8710314fd1ebed0fd158b043e6"
          "e7c9a76d62c6ba1e1d411a730902309ec676dd491433c6ef66c8f116233d6ce7" },
        { "f47be3a2b019d1beededf5b80c",
          "b94292625caa28c7be24a0997eb7328062a76d9b529c0f1d568f850df6d569b5"
          "e84df07e9e246be232033ffac3adf2d18f92ab9dacfc0ecf08aff7145f0b833b" },
        { "b1715f782ff02c6b88937f054116",
          "ee1a56ee78182ec41d2c3ab33d4c41871d437c5c1ca060ee9e219cb83689b4e5"
          "a4174dfdab5d1d1096a31a7c8d3abda75c1b5e6da97e1814901c505b0bc07f25" },
        { "9bcd5262868cd9c8a96c9e82987f03",
          "2e07662a001b9755ae922c8e8a95756db5341dc0f2e62ae1cf827038f33ce055"
          "f63ad5c00b65391428434ddc01e5535e7fecbf53db66d93099b8e0b7e44e4b25" },
        { "cd67bd4054aaa3baa0db178ce232fd5a",
          "0d8521f8f2f3900332d1a1a55c60ba81d04d28dfe8c504b6328ae787925fe018"
          "8f2ba91c3a9f0c1653c4bf0ada356455ea36fd31f8e73e3951cad4ebba8c6e04" },
        { "6ba004fd176791efb381b862e298c67b08",
          "112e19144a9c51a223a002b977459920e38afd4ca610bd1c532349e9fa7c0d50"
          "3215c01ad70e1b2ac5133cf2d10c9e8c1a4c9405f291da2dc45f706761c5e8fe" },
        { "c6a170936568651020edfe15df8012acda8d",
          "c36c100cdb6c8c45b072f18256d63a66c9843acb4d07de62e0600711d4fbe64c"
          "8cf314ec3457c90308147cb7ac7e4d073ba10f0ced78ea724a474b32dae71231" },
        { "61be0c9f5cf62745c7da47c104597194db245c",
          "b379249a3ca5f14c29456710114ba6f6136b34c3fc9f6fb91b59d491af782d6b"
          "237eb71aaffdd38079461cf690a46d9a4ddd602d19808ab6235d1d8aa01e8200" },
        { "e07056d4f7277bc548099577720a581eec94141d",
          "59f1856303ff165e2ab5683dddeb6e8ad81f15bb578579b999eb5746680f22cf"
          "ec6dba741e591ca4d9e53904837701b374be74bbc0847a92179ac2b67496d807" },
        { "67ebda0a3573a9a58751d4169e10c7e8663febb3a8",
          "13963f81cfabfca71de4739fd24a10ce3897bba1d716907fc0a28490c192a7fc"
          "3ccb8db1f91af7a2d250d6617f0dfd1519d221d618a02e3e3fa9041cf35ed1ea" },
        { "63e09db99eb4cd6238677859a567df313c8520d845b4",
          "9083e5348b08eb9810b2d15781d8265845410de54fe61750d4b93853690649ad"
          "c6e72490bc2b7c365e2390573d9414becc0939719e0cb78eca6b2c80c2fda920" },
        { "f3e06b4bd79e380a65cb679a98ccd732563cc5ebe892e2",
          "6b315f106b07c59eedc5ab1df813b3c0b903060e7217cc010e9070278512a885"
          "008dac8b2472a521e77835a7f4deadc1d591aa23b624b69948a99bb60121c54e" },
        { "16b17074d3e3d97557f9ed77d920b4b1bff4e845b345a922",
          "6884134582a760046433abcbd53db8ff1a89995862f305b887020f6da6c7b903"
          "a314721e972bf438483f452a8b09596298a576c903c91df4a414c7bd20fd1d07" },
        { "3edf93251349d22806bed25345fd5c190aac96d6cdb2d758b8",
          "299e0daf6605e5b0c30e1ec8bb98e7a3bd7b33b388bdb457452dab509594406c"
          "8e7b841e6f4e75c8d6fbd614d5eb9e56c359bfafb4285754787ab72b46dd33f0" },
        { "b2d5a14f01e6b778888c562a059ec819ad89992d16a09f7a54b4",
          "ab2e7d745d8ad393439af2a3fbc9cdc25510d4a04e78b526e12b1c0be3b22966"
          "872ebe652e2f46ed5c5acecd2f233a9175dd295ebeb3a0706fc66fa1b137042b" },
        { "844b66f12ba0c5f9e92731f571539d1eef332e1549a49dbfa4c6de",
          "c3f9c5781925774783ae9d839772d7513dfcea8c5af8da262c196f9fe80135b2"
          "b0c8c6ca0a1604e0a3460247620de20b299f2db7871982d27c2176ae5fa7ad65" },
        { "6b6cc692d39860b1f30203653e25d09c01e6a8043c1a9cb8b249a41e",
          "2e5263d9a4f21b210e0e161ed39df44102864325788647261a6e70ea4b1ee0ab"
          "b57b57499bc82158d82336dd53f1ef4464c6a08126e138b2cc0892f765f6af85" },
        { "ab1fc9ee845eeb205ec13725daf1fb1f5d50629b14ea9a2235a9350a88",
          "72d188a9df5f3b00057bca22c92c0f8228422d974302d22d4b322e7a6c8fc3b2"
          "b50ec74c6842781f29f7075c3d4bd065878648846c39bb3e4e2692c0f053f7ed" },
        { "594ed82acfc03c0e359cc560b8e4b85f6ee77ee59a70023c2b3d5b3285b2",
          "5ef322cb4014ecbb713a13659612a222225984d31c187debc4459ba7901f03da"
          "c775400acfe3510b306b79894fb0e8437b412150c9193ee5a2164306ebb78301" },
        { "f2c66efbf2a76c5b041860ea576103cd8c6b25e50eca9ff6a2fa88083fe9ac",
          "7978f93ef7ed02c4a24abecba124d14dd214e1492ff1e168304c0eab89637da0"
          "f7a569c43dc4562bdb9404a018b6314fe0eebaccfb25ba76506aa7e9dcd956a7" },
        { "8ccb08d2a1a282aa8cc99902ecaf0f67a9f21cffe28005cb27fcf129e963f99d",
          "4551def2f9127386eea8d4dae1ea8d8e49b2add0509f27ccbce7d9e950ac7db0"
          "1d5bca579c271b9f2d806730d88f58252fd0c2587851c3ac8a0e72b4e1dc0da6" }
    };

    for (const auto& p : test_vectors) {
        die_unequal(sha512_hex(tlx::parse_hexdump(p.first)), p.second);
    }
}

void test_md5() {
    die_unequal(
        md5_hex(""),
        "d41d8cd98f00b204e9800998ecf8427e");
    die_unequal(
        md5_hex("a"),
        "0cc175b9c0f1b6a831c399e269772661");
    die_unequal(
        md5_hex("abc"),
        "900150983cd24fb0d6963f7d28e17f72");
    die_unequal(
        md5_hex("message digest"),
        "f96b697d7cb7938d525a2f31aaf161d0");
    die_unequal(
        md5_hex("abcdefghijklmnopqrstuvwxyz"),
        "c3fcd3d76192e4007dfb496cca67e13b");
    die_unequal(
        md5_hex("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"),
        "d174ab98d277d9f5a5611c2c9f419d9f");
    die_unequal(
        md5_hex("12345678901234567890123456789012345678901234567890123456789012345678901234567890"),
        "57edf4a22be3c955ac49da2e2107b67a");
}

void test_sha1() {
    die_unequal(
        sha1_hex(""),
        "da39a3ee5e6b4b0d3255bfef95601890afd80709");
    die_unequal(
        sha1_hex("abc"),
        "a9993e364706816aba3e25717850c26c9cd0d89d");
    die_unequal(
        sha1_hex("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"),
        "761c457bf73b14d27e9e9265c46f4b4dda11f940");
    die_unequal(
        sha1_hex("12345678901234567890123456789012345678901234567890123456789012345678901234567890"),
        "50abf5706a150990a08b2c5ea40fa0e585554732");
}

int main() {
    test_sha256();
    test_sha512();
    test_md5();
    test_sha1();

    return 0;
}

/******************************************************************************/
