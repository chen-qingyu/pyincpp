/**
 * @file examples_main.cpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief 容器类应用例子 主函数
 * @version 1.0
 * @date 2023.01.12
 *
 * @copyright Copyright (c) 2023
 *
 */

void simulate_bank_queuing(int win_num, int serv_time);

int main(void)
{
    simulate_bank_queuing(4, 50); // 模拟4个窗口，50个时间单位

    return 0;
}
