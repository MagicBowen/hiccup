#include "catch2/catch.hpp"
#include "stubs/device.h"

using namespace ChipSdkStubs;

namespace {
    std::unordered_map<PhyPortId, LinkStatus, PhyPortIdHash> link_status_map;
    void link_status_callback(int chip_id, int port_id, LinkStatus status) {
        link_status_map[{chip_id, port_id}] = status;
    }
}

TEST_CASE( "Test chip sdk interfaces" ) {
    Device& device = Device::GetInstance();
    device.AddChip(SwitchChip{0, 2, {PhyPort{0, 0, LINK_DOWN}, PhyPort{1, 0, LINK_UP}}});
    device.AddChip(SwitchChip{1, 1, {PhyPort{0, 0, LINK_UP}}});

    SwitchChip chips[CHIP_SDK_CHIP_MAX];
    int chip_num = 0;
    REQUIRE(chip_sdk_init(chips, &chip_num) == CHIP_SDK_SUCCESS);
    REQUIRE(chip_num == 2);
    REQUIRE(chips[1].chip_id == 1);
    REQUIRE(chips[1].numOfPorts == 1);
    REQUIRE(chips[1].ports[0].port_id == 0);
    REQUIRE(chips[1].ports[0].speed == 0);
    REQUIRE(chips[1].ports[0].status == LINK_UP);


    Mac mac = {{0x00, 0x01, 0x02, 0x03, 0x04, 0x05}};
    REQUIRE(chip_sdk_set_mac(0, 0, &mac) == CHIP_SDK_SUCCESS);

    auto mac_ptr = device.GetMac(0, 0);
    REQUIRE(mac_ptr != nullptr);
    REQUIRE(mac_ptr->addr[0] == 0x00);
    REQUIRE(mac_ptr->addr[1] == 0x01);
    REQUIRE(mac_ptr->addr[2] == 0x02);
    REQUIRE(mac_ptr->addr[3] == 0x03);
    REQUIRE(mac_ptr->addr[4] == 0x04);
    REQUIRE(mac_ptr->addr[5] == 0x05);

    REQUIRE(chip_sdk_register_link_status_callback(link_status_callback) == CHIP_SDK_SUCCESS);
    REQUIRE(link_status_map.empty());
    device.SetLinkStatus(0, 0, LINK_UP);
    REQUIRE(link_status_map.size() == 1);
    REQUIRE(link_status_map[{0, 0}] == LINK_UP);

    device.SetLinkStatus(0, 1, LINK_UP);
    REQUIRE(link_status_map.size() == 1);
    device.SetLinkStatus(0, 0, LINK_DOWN);
    REQUIRE(link_status_map.size() == 1);
    REQUIRE(link_status_map[{0, 0}] == LINK_DOWN);
}
