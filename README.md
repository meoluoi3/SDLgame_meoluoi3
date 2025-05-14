# Game Bắn Súng 2D: Top-down shooter
## 1. Thông tin Sinh viên

    Tên: Hoàng Đức Nhuận

    Mã sinh viên: 24021590

## 2. Giới thiệu Game

Đây là một game bắn súng 2D cơ bản được phát triển bằng thư viện SDL2. Game cung cấp hai chế độ chơi chính để người chơi trải nghiệm: chế độ Sinh Tồn (Survivor) và chế độ Khám phá Hầm Ngục (Dungeon - chưa hoàn thiện).
## 3. Ngưỡng điểm mục tiêu

Tôi tin rằng project này xứng đáng đạt được mức điểm 9 - 9.5.
## 4. Lý do bảo vệ ngưỡng điểm

Dựa trên barem đánh giá và các tính năng đã triển khai, tôi xin trình bày các lý do để bảo vệ mức điểm mục tiêu:

    Cấu trúc project rõ ràng: Code được tổ chức module hóa thành nhiều file .h/.cpp riêng biệt theo chức năng.

    Quản lý đối tượng động: Sử dụng danh sách liên kết (linked list) để quản lý linh hoạt các đối tượng game.

    Hệ thống Input: Xử lý đầu vào từ cả bàn phím và chuột.

    Đồ họa (SDL2): Load/render texture, hỗ trợ xoay texture, vẽ UI (thanh máu, HUD), lớp nền và hiệu ứng hình ảnh.

    Gameplay cốt lõi: Di chuyển, bắn súng, hệ thống vũ khí đa dạng với thuộc tính riêng, chuyển đổi/nạp đạn, AI kẻ địch cơ bản, xử lý va chạm, tính điểm.

    Các thuật toán/kỹ thuật: Phát hiện va chạm AABB, tính toán vector/góc, giới hạn vị trí, giới hạn tốc độ khung hình (FPS capping) tùy chỉnh, State Machine quản lý trạng thái game.

    Tính năng mở rộng nổi bật: Hai chế độ chơi riêng biệt (Survivor, Dungeon), hệ thống Menu và Cài đặt đầy đủ tương tác bằng cả phím và chuột, Camera follow mượt mà trong chế độ Dungeon, Tutorial tương tác kết hợp hướng dẫn và thực hành.

## 5. Nguồn tham khảo

    Tài liệu của giảng viên: Các bài giảng và tài liệu được cung cấp trong môn học.

    Bạn bè trong lớp: Tôi đã nhận được sự hỗ trợ từ các bạn trong lớp trong việc thảo luận và giải thích rõ hơn các concept lập trình và thuật toán cần thiết cho project.

    Tutorials:

        https://lazyfoo.net/tutorials/SDL/index.php

        https://www.parallelrealities.co.uk/

        https://www.youtube.com/@MikeShah

        https://www.youtube.com/@CarlBirch

        reddit.com

    Assets (Images, Sounds):

        Ảnh: Genshin Impact, www.kenney.nl (https://www.youtube.com/watch?v=sfsJJOq1aYY), [Liệt kê thêm nguồn ảnh khác nếu có]

        Âm thanh: Sưu tầm trên mạng ([Liệt kê URL cụ thể nếu nhớ])

    Mức độ sử dụng AI:

        Tôi đã sử dụng AI như một công cụ hỗ trợ trong quá trình làm project. AI giúp tôi giải thích các khái niệm phức tạp trong SDL2, tìm hiểu cách triển khai một số tính năng, và hỗ trợ debug khi gặp lỗi. AI đóng vai trò là người đồng hành và trợ lý học tập, giúp tôi hiểu sâu hơn và đẩy nhanh tiến độ phát triển, nhưng toàn bộ logic cốt lõi và việc tích hợp các thành phần đều do tôi tự thực hiện.

        Lưu ý: Tính năng "Load map từ file" được gợi ý từ chatbot nhưng chưa kịp triển khai vào project này.

## 6. Điểm nổi bật để hướng tới điểm 9-10

Để đạt được mức điểm cao, game của tôi có những điểm nổi bật sau:

    Hai chế độ chơi hoàn toàn khác biệt: Mang lại hai trải nghiệm gameplay khác nhau rõ rệt về không gian và mục tiêu.

    Hệ thống vũ khí chi tiết: Triển khai nhiều loại vũ khí với thuộc tính riêng, cơ chế nạp đạn chủ động và thay đổi hình ảnh người chơi theo vũ khí.

    Hệ thống Menu và Cài đặt đầy đủ tính năng: Cung cấp bộ menu hoàn chỉnh với tùy chỉnh chi tiết, nâng cao trải nghiệm người dùng.

    Camera follow mượt mà: Kỹ thuật camera ghost trong chế độ Dungeon giúp việc di chuyển và khám phá bản đồ lớn trở nên chuyên nghiệp hơn.

    Tutorial tích hợp gameplay: Chế độ Tutorial cho phép người chơi thực hành ngay lập tức các thao tác được hướng dẫn.

    Quản lý bộ nhớ động hiệu quả: Sử dụng linked list tối ưu hóa việc sử dụng bộ nhớ.
