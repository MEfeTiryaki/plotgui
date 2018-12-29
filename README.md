# plotgui
Qt5 ve Qt plot tabanlı, basit bir plot uygulaması. Şimdilik catkin üzerinden projenize ekleyip kullanabilirsiniz.


## QT5 yükleme
Qt5'i kaynak kodundan yüklemek için [sitesinden](https://www.qt.io/download) kaynak kodunu indirin.Ardından sıkıştırılmış dosyayı uygun bir yere açın (qt sürümü farklı olabilir).
```bash
 tar -xvzf qt-everywhere-src-5.12.0.tar.gz
```
Ardından açtığınız klasörü düzenlemek için aşağıdaki kodu çalıştırın.
```bash
cd qt-everywhere-src-5.12.0
./configure
```
Şimdi derleyebiliriz. Bu adım biraz uzun sürecektir(bilgisayara bağlı olarak 30dk-1sa). "Open source" seçmek için "o" ya, ardından da "lisansı" kabul ediyorum anlamında "y" ye basacaksınız.
```bash
make
```
ardından yordamlıkları "/usr/local/Qt-5.12.0" yüklemek için aşağıdaki kodu çalıştırın. Önceki kadar uzun olmasa da bu da biraz vakit alacaktır.
```bash
sudo make install
```

ardından ".bahrc"'nize aşağıdaki satırları ekleyin

``` bash
PATH=/usr/local/Qt-5.12.0/bin:$PATH
export PATH
```

Qt5 başarıyla yüklenmiş olmalı. 
## QT5 için Cmake

Şu [arkadaşın](https://stackoverflow.com/questions/47471629/fail-to-use-q-object-macro-in-cmake-project`) yanıtı olmasa buralar gelemezdim.

## Kullanımı

![örnek](https://github.com/MEfeTiryaki/plotgui/blob/master/resource/plot_example.png)
