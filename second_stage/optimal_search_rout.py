class solver:
    def __init__(self, field_size):
        self.w, self.h = field_size

    def solve(self, telemetry):
        self.x = telemetry["X"]
        self.z = telemetry["Z"]
        self.psi = telemetry["Heading"]
        return v, roll, scan
