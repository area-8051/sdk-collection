/********************************** (C) COPYRIGHT *******************************
 * File Name          : app.h
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2021/11/18
 * Description        :
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#ifndef app_H
#define app_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define APP_NODE_EVT                   (1 << 0)
#define APP_NODE_PROVISION_EVT         (1 << 1)
#define APP_DELETE_NODE_TIMEOUT_EVT    (1 << 2)
#define APP_DELETE_LOCAL_NODE_EVT      (1 << 3)
#define APP_DELETE_NODE_INFO_EVT       (1 << 4)

#define CMD_PROVISION_INFO                 0xA0
#define CMD_PROVISION_INFO_ACK             0x80
#define CMD_PROVISION                      0xA1
#define CMD_PROVISION_ACK                  0x81
#define CMD_DELETE_NODE                0xA2
#define CMD_DELETE_NODE_ACK            0x82
#define CMD_DELETE_NODE_INFO           0xA3
#define CMD_LOCAL_RESET                0xAF

#define PERIPHERAL_CMD_LEN             1
#define PROVISION_NET_KEY_LEN          16
#define ADDRESS_LEN                    2

// 设置配网信息命令，包含 1字节命令码+1字节控制字+4字节iv index+1字节更新标志flag
#define PROVISION_INFO_DATA_LEN            (PERIPHERAL_CMD_LEN + 1 + 4 + 1)
// 设置配网信息命令应答，包含 1字节命令码+1字节状态码+4字节iv index+1字节更新标志flag
#define PROVISION_INFO_ACK_DATA_LEN        (PERIPHERAL_CMD_LEN + 1 + 4 + 1)
// 设置配网命令，包含 1字节命令码+16字节网络密钥+2字节网络地址
#define PROVISION_DATA_LEN                 (PERIPHERAL_CMD_LEN + PROVISION_NET_KEY_LEN + ADDRESS_LEN)
// 设置配网命令应答，包含 1字节命令码+2字节网络地址+1字节状态码
#define PROVISION_ACK_DATA_LEN             (PERIPHERAL_CMD_LEN + ADDRESS_LEN + 1)
// 删除节点命令，包含 1字节命令码+2字节需要删除的节点地址
#define DELETE_NODE_DATA_LEN           (PERIPHERAL_CMD_LEN + ADDRESS_LEN)
// 删除节点命令应答，包含 1字节命令码
#define DELETE_NODE_ACK_DATA_LEN       (PERIPHERAL_CMD_LEN)
// 删除存储的节点信息命令，包含 1字节命令码
#define DELETE_NODE_INFO_DATA_LEN      (PERIPHERAL_CMD_LEN)
// 本地复位命令，包含 1字节命令码
#define LOCAL_RESET_DATA_LEN           (PERIPHERAL_CMD_LEN)
// 状态码定义
#define STATUS_SUCCESS                     0x00
#define STATUS_TIMEOUT                     0x01
#define STATUS_NOMEM                       0x02
#define STATUS_INVALID                     0x03

/******************************************************************************/

typedef struct
{
    uint16_t node_addr;
    uint16_t elem_count;
    uint16_t net_idx;
    uint16_t retry_cnt : 12,
        fixed : 1,
        blocked : 1;

} node_t;

typedef union
{
    struct
    {
        uint8_t cmd;         /* 命令码 CMD_PROVISION_INFO */
        uint8_t set_flag;    /* 控制字 为1表示设置，为0表示查询*/
        uint8_t iv_index[4]; /* iv index */
        uint8_t flag;        /* Net key refresh flag */
    } provision_info;        /* 配网信息命令 */
    struct
    {
        uint8_t cmd;         /* 命令码 CMD_PROVISION_INFO_ACK */
        uint8_t status;      /* 状态码*/
        uint8_t iv_index[4]; /* iv index */
        uint8_t flag;        /* Net key refresh flag */
    } provision_info_ack;    /* 配网信息命令应答 */
    struct
    {
        uint8_t cmd;                            /* 命令码 CMD_PROVISION */
        uint8_t net_key[PROVISION_NET_KEY_LEN]; /* 后续数据长度 */
        uint8_t addr[ADDRESS_LEN];              /* 配网地址 */
    } provision;                                /* 配网命令 */
    struct
    {
        uint8_t cmd;               /* 命令码 CMD_PROVISION_ACK */
        uint8_t addr[ADDRESS_LEN]; /* 配网地址 */
        uint8_t status;            /* 状态码备用 */
    } provision_ack;               /* 配网命令应答 */
    struct
    {
        uint8_t cmd;                /* 命令码 CMD_DELETE_NODE */
        uint8_t addr[ADDRESS_LEN]; /* 删除地址 */
    } delete_node;                  /* 删除节点命令 */
    struct
    {
        uint8_t cmd;                /* 命令码 CMD_DELETE_NODE_ACK */
    } delete_node_ack;              /* 删除节点命令应答 */
    struct
    {
        uint8_t cmd;                /* 命令码 CMD_DELETE_NODE_INFO */
    } delete_node_info;             /* 删除存储的节点信息命令 */
    struct
    {
        uint8_t cmd;                /* 命令码 CMD_LOCAL_RESET */
    } local_reset;                  /* 本地恢复出厂设置命令 */
    struct
    {
        uint8_t buf[20]; /* 接收数据包*/
    } data;
}app_mesh_manage_t;

void App_Init(void);

void App_peripheral_reveived(uint8_t *pValue, uint16_t len);

/******************************************************************************/

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
