# Plot Gui
English version is below.

C++ projeleri için kullanışlı bir plot yordamlığı olmaması dolayısıyla bu yordamlığı hazırladım. Herşeyden önce (QCP)[
https://www.qcustomplot.com/] ekibine alkışlarımı bildirmek isterim, onların çalışması olmasa bu yordamlığı hazırlayamazdım. QCP çok esnek plot arayüzü araçları sunsa da sıradan kullanıcılar için karmaşık gelebilir. İşbu yordamlığı, kullanımı kolaylaştırmak adına mümkün oldukça Matlab plot yordamlarına benzer şekilde tasarlamaya ve benzer işlevleri vermeğe çalıştım. 

Yordamlık şimdilik "Catkinized" olarak derlenebiliyor. İleride "make;make install" ile kaynak koddan kurulumda eklemeyi planlıyorum(Nasıl yapacağımı öğrenir öğrenmez!:D)


Gereksinimler:
* Eigen (3.2.92 sürümü, ubuntu 16.04'da yüklü gelir.)
* QT(5.12.0 sürümü, cmake şimdilik kaynak koddan yüklemiş qt yordamlarını bulabiliyor.)


Planlanan Eklemeler
* a
 * a

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
