import numpy as np

def pixel_to_world(u, v, camera_position, camera_orientation, drop_height, camera_intrinsics):
    """
    Преобразует пиксельные координаты в мировые координаты с учетом ориентации камеры и высоты.

    Args:
        u (float): Координата x пикселя относительно центра изображения.
        v (float): Координата y пикселя относительно центра изображения.
        camera_position (tuple): (Xc, Yc, Zc) - положение камеры в мировых координатах.
        camera_orientation (tuple): (roll, pitch, yaw) - углы ориентации камеры (в радианах).
        drop_height (float): Zb - Высота точки доставки в мировых координатах.
        camera_intrinsics (dict): {
            'fx': float,  # Фокусное расстояние по оси x
            'fy': float,  # Фокусное расстояние по оси y
            'cx': float,  # Координата x центра изображения
            'cy': float   # Координата y центра изображения
        }

    Returns:
        tuple: (x, y, z) - Мировые координаты точки доставки.  None если не удалось вычислить.
    """

    Xc, Yc, Zc = camera_position
    roll, pitch, yaw = camera_orientation
    fx = camera_intrinsics['fx']
    fy = camera_intrinsics['fy']
    cx = camera_intrinsics['cx']
    cy = camera_intrinsics['cy']


    # 1. Нормализованные координаты изображения
    x = (u - cx) / fx
    y = (v - cy) / fy

    # 2. Матрицы поворота
    def rotation_matrix_z(angle):
        return np.array([
            [np.cos(angle), -np.sin(angle), 0],
            [np.sin(angle), np.cos(angle), 0],
            [0, 0, 1]
        ])

    def rotation_matrix_x(angle):
        return np.array([
            [1, 0, 0],
            [0, np.cos(angle), -np.sin(angle)],
            [0, np.sin(angle), np.cos(angle)]
        ])

    def rotation_matrix_y(angle):
        return np.array([
            [np.cos(angle), 0, np.sin(angle)],
            [0, 1, 0],
            [-np.sin(angle), 0, np.cos(angle)]
        ])


    Rz = rotation_matrix_z(-yaw)  # Инвертируем угол курса
    Rx = rotation_matrix_x(roll)
    Ry = rotation_matrix_y(pitch)

    # Итоговая матрица поворота
    R = Rz @ Rx @ Ry  # Порядок важен

    # 3. Вектор направления в системе координат камеры
    V_camera = np.array([x, y, 1])

    # 4. Вектор направления в системе координат мира
    V_world = R @ V_camera

    # 5. Вычисление мировых координат
    delta_z = drop_height - Zc

    # Проверка деления на ноль
    if abs(V_world[2]) < 1e-6:  # epsilon для избежания деления на ноль
        print("Предупреждение: Z-компонента вектора направления в мировых координатах близка к нулю.  Невозможно вычислить координаты.")
        return None

    s = delta_z / V_world[2]
    real_coords_relative = s * V_world
    real_coords_world = (Xc + real_coords_relative[0], Yc + real_coords_relative[1], drop_height)

    return real_coords_world


# Пример использования
if __name__ == '__main__':
    # Данные
    u = 10  # Смещение по x от центра изображения
    v = -5   # Смещение по y от центра изображения
    camera_position = (324.624321, 465.566588, 84.785361)  # Xc, Yc, Zc
    camera_orientation = (23.5240, -55.8101, 3.7997)  # roll, pitch, yaw (в радианах)
    drop_height = 17.075479  # Zb

    camera_intrinsics = {
        'fx': 324.624321,
        'fy': 465.566588,
        'cx': 320,
        'cy': 240
    }

    # Вычисление мировых координат
    world_coordinates = pixel_to_world(u, v, camera_position, camera_orientation, drop_height, camera_intrinsics)

    if world_coordinates:
        print("Мировые координаты точки доставки:", world_coordinates)
