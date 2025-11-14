# ITURO_2022
ITURO 2022 robot olimpiyatları için üzerinde çalıştığım robotlar ve kodları


🤖 İTÜRO 2022 Robot Olimpiyatları Projeleri
Bu depo, İTÜ Robot Olimpiyatları (İTÜRO) 2022 yarışmaları için geliştirilen iki farklı otonom robot projesinin tüm kaynak kodlarını ve dokümantasyonunu içermektedir.

🏆 Kapsanan Projeler
Bu depoda, farklı kategorilerde yarışan iki ana projenin kodları yer almaktadır:

Trafik Robotu (Bedel): Trafik Kategorisi.

Lojistik Robot Sistemi (NAVIGATOR & GRIPPER): Lojistik Kategorisi.

1. 🚦 Trafik Robotu (Bedel)
Proje Özeti
Bedel, İTÜRO 2022 Trafik Kategorisi'nde yarışmak üzere tasarlanmış, otonom bir trafik takip robotudur. Robotun temel görevi, yarışma pisti üzerindeki trafik tabelalarındaki işaretleri doğru bir şekilde algılamak ve bu işaretlere uygun manevraları yaparak pisti tamamlamaktır.

Kullanılan Teknolojiler
Mikrodenetleyici: Arduino (Sensör ve Motor Kontrolü)

Görüntü İşleme: Python (Tabela Tanıma ve Karar Verme)

Donanım: Çizgi sensörleri, motorlar, kamera modülü.

📂 Kod Konumu
Trafik Robotu'na ait tüm kodlar ve dökümanlar aşağıdaki dizinde yer almaktadır:

./01_trafik_robotu/

2. 🔗 Lojistik Robot Sistemi (NAVIGATOR & GRIPPER)
Proje Özeti
Bu proje, İTÜRO 2022 Lojistik Kategorisi için geliştirilmiş, entegre ve iki robotlu bir lojistik çözümüdür. Sistem, rota planlama ve hassas yük taşıma görevini iki robotun iş birliğiyle gerçekleştirir:

NAVIGATOR (Rota Çizici): Pist üzerindeki tabelalardan koordinatları okur ve rotayı hesaplar.

GRIPPER (Kıskaçlı Taşıyıcı): NAVIGATOR'dan Bluetooth ile aldığı rota bilgisiyle yükü alıp istenen noktaya teslim eder.

Kullanılan Teknolojiler
Kontrol: Arduino (Motor, Servo ve Temel Kontrol)

Zeka & Navigasyon: Python (Rota Planlama, Görüntü İşleme, Kinematik Kontrol)

İletişim: Bluetooth (Robotlar arası kablosuz veri aktarımı)
