# CPU_Surec_Zamanlayici_Simulasyonu
CPU Zamanlayıcı sistemini C dilinde simüle eden program

Bu projenin amacı, işletim sistemlerinde kullanıla CPU zamanlama algoritmalarını C programlama diliyle simüle etmek ve bu algoritmaların sonuçlarını karşılaştırmaktır. 

Bilgisayarda aynı anda birden fazla program çalıştırma ihtiyacı duyabiliriz. Ancak işlemci, belirli bir anda sınırlı sayıda işlemci çalıştırabilir. İşletim sistemi, hangi işlemin ne zaman çalışacağını belirlemek için zamanlama algoritmalarından faydalanır. 


Bu projede 3 farklı zamanlama algoritmasından yararlanırlmıştır;

1- FCFS (First Come First Served) yani ilk gelen ilk hizmet görür.

2- SJF (Short Job First) yani CPU süresi kısa olan önce hizmet görür.

3- Round Robin her sürece belirli bir zaman dilimi verilir, bu eşittir. İşlemler o süre bitene kadar döngüsel olarak devamlı döner, işlemi tamamen biten döngüden ayrılır.



Program kullanılırken süreçlerin geliş zamanlarını ve CPU'da ihtiyaç duydukları süreleri alır. Ardından her algoritmayı ayrı ayrı çalıştırarak süreçlerin çalışma sırasını, bitiş zamanlarını, bekleme sürelerini, dönüş sürelerini ve bağlam değişimi sayılarını hesaplar. 

Son olarak algoritmaların ortalama bekleme süreleri karşılaştırılarak en düşük bekleme süresine sahip algoritma ekrana yazdılır.
