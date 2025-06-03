import random
import matplotlib.pyplot as plt

# 页面置换算法模拟
class PageReplacementSimulator:
    def __init__(self, page_references):
        self.page_references = page_references  # 页面引用串

    # FIFO算法
    def fifo(self, frame_count):
        page_frames = []  # 当前内存中的页面
        page_faults = 0   # 缺页次数
        for page in self.page_references:
            if page not in page_frames:
                if len(page_frames) < frame_count:
                    page_frames.append(page)
                else:
                    page_frames.pop(0)
                    page_frames.append(page)
                page_faults += 1
        return page_faults

    # EClock算法（增强型时钟算法）
    def eclock(self, frame_count):
        page_faults = 0
        if frame_count == 0:
            return len(self.page_references)  # 如果帧数为0，缺页次数等于引用次数

        page_frames = []  # 当前内存中的页面
        used = []         # 记录页面是否被使用过
        modified = []     # 记录页面是否被修改过
        pointer = 0       # 时钟指针

        for page in self.page_references:
            # 如果页面已经在内存中
            if page in page_frames:
                idx = page_frames.index(page)
                used[idx] = True
                # 假设页面被访问时有30%的概率被修改
                if random.random() < 0.3:
                    modified[idx] = True
            else:
                page_faults += 1
                if len(page_frames) < frame_count:
                    # 如果内存未满，添加新页面
                    page_frames.append(page)
                    used.append(False)
                    modified.append(False)
                else:
                    # 如果内存已满，使用时钟算法替换页面
                    while True:
                        # 如果当前页面未被使用过，则可以替换
                        if not used[pointer]:
                            # 如果页面被修改过，需要先写回（这里只模拟）
                            if modified[pointer]:
                                pass
                            # 替换页面
                            page_frames[pointer] = page
                            used[pointer] = True
                            modified[pointer] = False  # 新页面初始为未修改
                            pointer = (pointer + 1) % frame_count
                            break
                        else:
                            # 给页面第二次机会
                            used[pointer] = False
                            pointer = (pointer + 1) % frame_count
        return page_faults

# 主函数
if __name__ == "__main__":
    # 调整页面引用串范围，以增加缺页次数的变化
    sequential_references = [1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5]  # 顺序引用串，包含重复访问
    # 随机引用串：从1-20中随机选择20个数，增加引用的随机性
    random_references = [random.randint(1, 20) for _ in range(20)]  

    # 创建模拟器对象
    sequential_simulator = PageReplacementSimulator(sequential_references)
    random_simulator = PageReplacementSimulator(random_references)

    # 测试的帧数范围
    frame_counts = list(range(1, 11))

    # 记录缺页次数
    sequential_fifo_faults = []
    sequential_eclock_faults = []
    random_fifo_faults = []
    random_eclock_faults = []

    # 运行模拟并收集数据
    for frames in frame_counts:
        sequential_fifo_faults.append(sequential_simulator.fifo(frames))
        sequential_eclock_faults.append(sequential_simulator.eclock(frames))
        random_fifo_faults.append(random_simulator.fifo(frames))
        random_eclock_faults.append(random_simulator.eclock(frames))

    # 绘制图表
    plt.figure(figsize=(12, 6))

    # 顺序引用串的图表
    plt.subplot(1, 2, 1)
    plt.plot(frame_counts, sequential_fifo_faults, label='FIFO')
    plt.plot(frame_counts, sequential_eclock_faults, label='EClock')
    plt.title('Sequential Page References')
    plt.xlabel('Frame Count')
    plt.ylabel('Page Faults')
    plt.legend()
    plt.grid(True)

    # 随机引用串的图表
    plt.subplot(1, 2, 2)
    plt.plot(frame_counts, random_fifo_faults, label='FIFO')
    plt.plot(frame_counts, random_eclock_faults, label='EClock')
    plt.title('Random Page References')
    plt.xlabel('Frame Count')
    plt.ylabel('Page Faults')
    plt.legend()
    plt.grid(True)

    # 调整子图间距
    plt.tight_layout()

    # 显示图表
    plt.show()