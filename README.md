# Plot Gui
English version is below.

C++ projeleri için kullanışlı bir plot yordamlığı olmaması dolayısıyla bu yordamlığı hazırladım. Herşeyden önce [QCP](
https://www.qcustomplot.com/) ekibine alkışlarımı bildirmek isterim, onların çalışması olmasa bu yordamlığı hazırlayamazdım. QCP çok esnek plot arayüzü araçları sunsa da sıradan kullanıcılar için karmaşık gelebilir. İşbu yordamlığı, kullanımı kolaylaştırmak adına mümkün oldukça Matlab plot yordamlarına benzer şekilde tasarlamaya ve benzer işlevleri vermeğe çalıştım. 

Yordamlık şimdilik "Catkinized" olarak derlenebiliyor. İleride "make;make install" ile kaynak koddan kurulumda eklemeyi planlıyorum(Nasıl yapacağımı öğrenir öğrenmez!:D)


Gereksinimler:
* Eigen (3.2.92 sürümü, ubuntu 16.04'da yüklü gelir.)
* QT (5.12.0 sürümü, cmake şimdilik kaynak koddan yüklemiş qt yordamlarını bulabiliyor.)


Planlanan Eklemeler
* Plot
  * Farklı çizgi türleri
  * Matlab benzeri renk kodlama
* Scatter
  * Farklı nokta Türleri
* Bar plot
* Subplot
  * subplot düzeni ayarlama
  * plotlar arası eksen bağlama
* Etkileşim
  * Zoom ve Move özellikleri için çift tıklama ile sıfırlama
  * Kare seçerek zoom yapma
* Legend
  * Legend yeri ayarlama
  * Legend kutusu ekleme
* Latex desteği
* Kullanım
  * QApplication'ı yordamlık içine çekip kullanıcının Qt bağlılığını azalt 
* Kurulum
  * cmake kurulumu ekleme
  * online Qt kurulumunu cmakelistle bulma
  


## Kullanımı

![örnek](https://github.com/MEfeTiryaki/plotgui/blob/master/resource/plot_example.png)
