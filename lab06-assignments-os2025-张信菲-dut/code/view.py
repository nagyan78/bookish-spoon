import matplotlib.pyplot as plt

class DiskArmSimulator:
    def __init__(self, current_cylinder, seek_direction, request_sequence):
        self.current_cylinder = current_cylinder
        self.seek_direction = seek_direction
        self.request_sequence = request_sequence.copy()

    def fcfs(self):
        current = self.current_cylinder
        sequence = [current]
        for cylinder in self.request_sequence:
            sequence.append(cylinder)
        return sequence

    def sstf(self):
        current = self.current_cylinder
        sequence = [current]
        remaining_requests = self.request_sequence.copy()
        while remaining_requests:
            shortest_distance = float('inf')
            next_cylinder = None
            for cylinder in remaining_requests:
                distance = abs(current - cylinder)
                if distance < shortest_distance:
                    shortest_distance = distance
                    next_cylinder = cylinder
            sequence.append(next_cylinder)
            remaining_requests.remove(next_cylinder)
            current = next_cylinder
        return sequence

    def scan(self):
        current = self.current_cylinder
        direction = self.seek_direction
        point = 0
        for i in range(len(self.request_sequence)):
            if self.request_sequence[i] <= current:
                point += 1
        sorted_cylinder = sorted(self.request_sequence)
        sequence = [current]
        if direction == 0:
            for i in range(point - 1, -1, -1):
                sequence.append(sorted_cylinder[i])
            sequence.append(0)
            for i in range(point, len(self.request_sequence)):
                sequence.append(sorted_cylinder[i])
        else:
            for i in range(point, len(self.request_sequence)):
                sequence.append(sorted_cylinder[i])
            sequence.append(200)
            for i in range(point - 1, -1, -1):
                sequence.append(sorted_cylinder[i])
        return sequence

    def cscan(self):
        current = self.current_cylinder
        direction = self.seek_direction
        point = 0
        for i in range(len(self.request_sequence)):
            if self.request_sequence[i] <= current:
                point += 1
        sorted_cylinder = sorted(self.request_sequence)
        sequence = [current]
        if direction == 0:
            for i in range(point - 1, -1, -1):
                sequence.append(sorted_cylinder[i])
            sequence.append(0)
            sequence.append(200)
            for i in range(len(self.request_sequence) - 1, point - 1, -1):
                sequence.append(sorted_cylinder[i])
        else:
            for i in range(point, len(self.request_sequence)):
                sequence.append(sorted_cylinder[i])
            sequence.append(200)
            sequence.append(0)
            for i in range(point - 1, -1, -1):
                sequence.append(sorted_cylinder[i])
        return sequence

    def look(self):
        current = self.current_cylinder
        direction = self.seek_direction
        point = 0
        for i in range(len(self.request_sequence)):
            if self.request_sequence[i] <= current:
                point += 1
        sorted_cylinder = sorted(self.request_sequence)
        sequence = [current]
        if direction == 0:
            for i in range(point - 1, -1, -1):
                sequence.append(sorted_cylinder[i])
            for i in range(point, len(self.request_sequence)):
                sequence.append(sorted_cylinder[i])
        else:
            for i in range(point, len(self.request_sequence)):
                sequence.append(sorted_cylinder[i])
            for i in range(point - 1, -1, -1):
                sequence.append(sorted_cylinder[i])
        return sequence

def plot_seek_curves(current_cylinder, seek_direction, request_sequence):
    simulator = DiskArmSimulator(current_cylinder, seek_direction, request_sequence)
    algorithms = {
        'FCFS': simulator.fcfs(),
        'SSTF': simulator.sstf(),
        'SCAN': simulator.scan(),
        'CSCAN': simulator.cscan(),
        'LOOK': simulator.look()
    }
    plt.figure(figsize=(12, 8))
    colors = ['blue', 'green', 'red', 'cyan', 'magenta']
    for idx, (alg_name, sequence) in enumerate(algorithms.items()):
        plt.subplot(3, 2, idx + 1)
        y = list(range(len(sequence)))
        plt.plot(sequence, y, marker='o', linestyle='-', color=colors[idx % len(colors)])
        plt.title(f'{alg_name} Seek Curve')
        plt.xlabel('Cylinder')
        plt.ylabel('Sequence')
        plt.grid(True)
        for i, txt in enumerate(sequence):
            plt.annotate(txt, (sequence[i], y[i]))
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    test_cases = [
        {
            "current_cylinder": 50,
            "seek_direction": 1,
            "request_sequence": [82, 170, 43, 140, 24, 16, 190]
        },
        {
            "current_cylinder": 100,
            "seek_direction": 0,
            "request_sequence": [150, 30, 120, 70, 180, 40, 200]
        },
        {
            "current_cylinder": 30,
            "seek_direction": 1,
            "request_sequence": [50, 10, 20, 40, 60, 70, 80]
        }
    ]
    for idx, test_case in enumerate(test_cases):
        print(f"Test Case {idx + 1}:")
        print(f"Current cylinder: {test_case['current_cylinder']}")
        print(f"Seek direction: {test_case['seek_direction']}")
        print(f"Request sequence: {test_case['request_sequence']}")
        plot_seek_curves(
            test_case['current_cylinder'],
            test_case['seek_direction'],
            test_case['request_sequence']
        )