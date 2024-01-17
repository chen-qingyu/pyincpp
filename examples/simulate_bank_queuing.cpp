/**
 * @file simulate_bank_queuing.cpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief 模拟银行多窗口排队，队列应用
 * @date 2023.01.12
 *
 * @copyright Copyright (C) 2023
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <ctime>
#include <iostream>

#include <mds/Deque.hpp>

// 顾客类
struct Customer
{
    int time; // 所需服务时长
};

const static int MAX_TIME = 10; // 最大所需服务时长

std::ostream& operator<<(std::ostream& os, const Customer& c)
{
    return os << "(time: " << c.time << ")";
}

// 为新到顾客确定最佳队列
int best_window(mds::Deque<Customer> windows[], int win_num)
{
    int min_size = windows[0].size(), opt_win = 0;
    for (int i = 1; i < win_num; i++) // 在所有窗口中
    {
        if (min_size > windows[i].size()) // 挑选出队列最短者
        {
            min_size = windows[i].size();
            opt_win = i;
        }
    }
    return opt_win;
}

// 按指定窗口数、服务总时间模拟银行业务
void simulate_bank_queuing(int win_num, int serv_time)
{
    mds::Deque<Customer>* windows = new mds::Deque<Customer>[win_num]; // 为每一窗口创建一个队列

    std::srand((unsigned int)std::time(nullptr));
    for (int now = 1; now <= serv_time; now++) // 每隔一个单位时间
    {
        std::cout << "Time: " << now << std::endl;
        if (rand() % (1 + win_num)) // 新顾客以一定的概率到达，概率正比于窗口数量
        {
            Customer c{1 + rand() % MAX_TIME};       // 新顾客到达，服务时长随机确定
            int win = best_window(windows, win_num); // 找出最佳（最短）的服务窗口
            windows[win].push_back(c);               // 新顾客加入对应的队列
            std::cout << "New customer: " << c << " at Window " << win << std::endl;
        }

        for (int i = 0; i < win_num; i++) // 分别检查
        {
            std::cout << "Window " << i << ": " << windows[i] << std::endl;
            if (!windows[i].is_empty()) // 各非空队列
            {
                if (--windows[i].front().time <= 0) // 队首顾客的服务时长减少一个单位
                {
                    windows[i].pop_front(); // 服务完毕的顾客出列
                }
            }
        }
        std::cout << std::endl;
    }

    delete[] windows;
}
