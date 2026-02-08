# Giới thiệu về SCD40
SCD40 là cảm biến khí CO2 dựa trên nguyên lý **quang âm (Photoacoustic)**. Công nghệ này cho phép thiết kế cảm biến cực kỳ nhỏ gọn nhưng vẫn đạt độ chính xác cao, vượt trội hơn các dòng cảm biến NDIR truyền thống.

## Thông số kỹ thuật
*   **Dải đo CO2:** 400 – 2.000 ppm (Độ chính xác: ±40 ppm + 5% giá trị đọc).
*   **Cảm biến tích hợp:** Nhiệt độ (±0.8°C) và Độ ẩm tương đối (±6%).
*   **Điện áp hoạt động:** 2.4V - 5.5V (Tối ưu ở **3.3V** cho ESP32).
*   **Giao tiếp:** I2C (Địa chỉ mặc định: `0x62`).
*   **Chế độ lấy mẫu:**
    *   **Normal Mode:** 5 giây/lần (Dòng tiêu thụ ≈ 15mA).
    *   **Low Power Mode:** 30 giây/lần (Dòng tiêu thụ ≈ 3.5mA).

# Sơ đồ kết nối

Hệ thống sử dụng bus I2C mặc định của ESP32 để đảm bảo tốc độ truyền tải và độ ổn định cao nhất.

| Chân SCD40 | Chân ESP32 | Chức năng |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Nguồn |
| **GND** | GND | Nối đất |
| **SCL** | GPIO 22 | I2C Clock |
| **SDA** | GPIO 21 | I2C Data |

---

# Tính năng mã nguồn
Mã nguồn được thiết kế để tối ưu hóa năng lượng dựa trên nhu cầu thực tế của người dùng:
1.  **Dynamic Mode Switching:** Cho phép chuyển đổi giữa chế độ đo nhanh (5s) và đo tiết kiệm điện (30s) mà không cần khởi động lại vi điều khiển.
2.  **Cơ chế an toàn:** Tự động gọi lệnh `stopPeriodicMeasurement()` trước khi chuyển chế độ đo để bảo vệ trạng thái nội bộ của cảm biến.
3.  **Kiểm tra trạng thái dữ liệu:** Sử dụng hàm `getDataReadyStatus()` để đảm bảo dữ liệu chỉ được đọc khi cảm biến đã hoàn thành chu kỳ đo, tránh nhiễu bus I2C.

## Lệnh điều khiển qua Serial Monitor (115200 baud):
*   `NORMAL`: Chuyển sang chế độ lấy mẫu **5 giây/lần** (Giám sát tức thời).
*   `LOW`: Chuyển sang chế độ lấy mẫu **30 giây/lần** (Tiết kiệm điện năng).
