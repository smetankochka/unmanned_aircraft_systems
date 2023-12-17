from math import degrees


class solver:

    def distanceToBorderX(self, x):
        return self.height - x >= 16 and x >= 16

    def distanceToBorderY(self, y):
        return self.width - y >= 16 and y >= 16

    def __init__(self, field_size):
        self.width, self.height = field_size
        self.isUp: bool = True
        self.lastScanningCoords = (-15, -15)
        self.v, self.roll, self.scan = 15, 0, 1
        self.downs = 0
        self.downValue = self.width // 183  # Количество полётов вниз
        self.goLeft = False

        self.rotationRight = True
        self.rotationLeft1 = False
        self.goForward = False
        self.rotationLeft2 = False

        self.scanRightBottomCorner = True
        self.scanRightTopCorner = True

    def solve(self, telemetry):

        x = telemetry["X"]
        y = telemetry["Z"]
        psi = telemetry["Heading"]
        # print(x, y, degrees(psi), self.downs)
        # Летим вверх
        if self.isUp:

            if self.downs < self.downValue:
                if x > self.height - 30 and int(degrees(psi)) not in range(179,
                                                                           181):  # Подлетаем к краю, снижаем скорость для виража
                    self.v = 15
                    self.roll = 20
                elif x > self.height - 30 and int(degrees(psi)) in range(179, 181):
                    self.isUp = False
                    self.downs += 1
                    self.v = 25
                    self.roll = 0
                else:
                    self.v = 25
                    self.roll = 0
            else:

                if self.goLeft:
                    if x > self.height - 30 and int(degrees(psi)) not in range(179,
                                                                               181):  # Подлетаем к краю, снижаем скорость для виража
                        self.v = 15
                        self.roll = -20
                    elif x > self.height - 30 and int(degrees(psi)) in range(179, 181):
                        self.isUp = False
                        self.downs += 1
                        self.v = 25
                        self.roll = 0
                    else:
                        self.v = 25
                        self.roll = 0
                else:  # летим направо
                    if x > self.height - 30 and int(degrees(psi)) not in range(179,
                                                                               181):  # Подлетаем к краю, снижаем скорость для виража
                        self.v = 15
                        self.roll = 20
                    elif x > self.height - 30 and int(degrees(psi)) in range(179, 181):
                        self.isUp = False
                        self.downs += 1
                        self.v = 25
                        self.roll = 0
                    else:
                        self.v = 25
                        self.roll = 0

        elif not self.isUp:

            if self.downs < self.downValue:  # Летим направо
                if x < 30 and int(degrees(psi)) not in range(-1,
                                                             1):  # Подлетаем к нижнему краю, снижаем скорость для виража
                    self.v = 15
                    self.roll = -20
                elif x > 30 and int(degrees(psi)) in range(-1, 1):
                    self.isUp = True
                    self.v = 25
                    self.roll = 0
                else:
                    self.v = 25
                    self.roll = 0
            else:
                if self.goLeft:
                    if x < 30 and int(degrees(psi)) not in range(359,
                                                                 361):  # Подлетаем к нижнему краю, снижаем скорость для виража
                        self.roll = 20
                        if self.downs == self.downValue:
                            self.v = 18.3
                    elif x > 30 and int(degrees(psi)) in range(359, 361):
                        self.isUp = True
                        self.v = 25
                        self.roll = 0
                    else:
                        self.v = 25
                        self.roll = 0
                else:  # летим направо на втором спуске вниз
                    if self.rotationLeft1 or self.rotationLeft2 or self.rotationRight or self.goForward:
                        if self.rotationRight and x < 30 and int(degrees(psi)) not in range(269,
                                                                                            271):  # Подлетаем к нижнему краю, снижаем скорость для виража
                            self.v = 15
                            self.roll = 20
                        elif self.rotationRight and x < 30 and int(degrees(psi)) in range(269,
                                                                                          271):  # Повернули направо на 90 градусов, теперь поворачиваем налево
                            self.rotationRight = False
                            self.rotationLeft1 = True
                            # self.isUp = True
                            self.v = 15
                            self.roll = -20
                        elif self.rotationLeft1 and x < 30 and int(degrees(psi)) not in range(89,
                                                                                              91):  # Поворачиваем налево
                            self.rotationRight = False
                            self.rotationLeft1 = True
                            # self.isUp = True
                            self.v = 15
                            self.roll = -20
                        elif self.rotationLeft1 and x < 30 and int(degrees(psi)) in range(89,
                                                                                          91):  # Повернул налево на 180 градусов, теперь нужно пролететь прямо около 60 метров
                            self.rotationLeft1 = False
                            self.goForward = True
                            self.v = 25
                            self.roll = 0
                        elif self.goForward and x < 30 and y < (self.width - 95):  # Летим
                            self.v = 25
                            self.roll = 0
                            self.goForward = True
                        elif self.goForward and x < 30 and (
                                y > (self.width - 95)):  # Пролетели нужную дистанцию, теперь повернём налево снова
                            self.v = 15
                            self.roll = -20
                            self.goForward = False
                            self.rotationLeft2 = True
                        elif self.rotationLeft2 and x < 30 and int(degrees(psi)) not in range(-1, 1):  # Поворачиваем
                            self.v = 15
                            self.roll = -20
                            self.goForward = False
                            self.rotationLeft2 = True
                        elif self.rotationLeft2 and int(degrees(psi)) in range(-1, 1):  # Повернули (теперь мы вроде как на крайней правой полосе)
                            self.v = 25
                            self.roll = 0
                            self.goForward = False
                            self.rotationLeft2 = False
                            self.isUp = True
                            self.goLeft = True

            print(x, y)

            if self.distanceToBorderX(x) and self.distanceToBorderY(y) and ((self.lastScanningCoords[0] - x)2 + (self.lastScanningCoords[1] - y)2) ** 0.5 > 60:
                self.scan = 1
                self.lastScanningCoords = (x, y)
            else:
                self.scan = 0
            return self.v, self.roll, self.scan
