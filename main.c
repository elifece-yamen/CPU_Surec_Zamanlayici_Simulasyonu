#include <stdio.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/*
    MAX:
    Programda kullanılabilecek en fazla süreç sayısını belirtir.
    Bu değer 20 olduğu için kullanıcı en fazla 20 süreç girebilir.

    MAX_BLOK:
    Gantt şemasında saklanabilecek en fazla zaman bloğu sayısını
    belirtir. Round Robin algoritmasında süreçler tekrar tekrar
    çalıştırılabildiği için birden fazla blok oluşabilir. */

	#define MAX 20
#define MAX_BLOK 10000

/* Yazan: [Zeynep Ecren AYDOĞAN]
   Algoritmalar çalışırken oluşan zaman aralıklarını Gantt şemasına kaydetmek için kullanılır. */
   
void blokEkle(int surecIndeksi, int baslangicZamani, int bitisZamani,
              int ganttSurecleri[], int ganttBaslangiclari[],
              int ganttBitisleri[], int *blokSayisi,
              int *baglamDegisimiSayisi, int *oncekiSurecIndeksi)
{
    if (surecIndeksi != -1)
    {
        if (*oncekiSurecIndeksi != -1 &&
            *oncekiSurecIndeksi != surecIndeksi)
            (*baglamDegisimiSayisi)++;

        *oncekiSurecIndeksi = surecIndeksi;
    }

    if (*blokSayisi > 0 &&
        ganttSurecleri[*blokSayisi - 1] == surecIndeksi &&
        ganttBitisleri[*blokSayisi - 1] == baslangicZamani)
    {
        ganttBitisleri[*blokSayisi - 1] = bitisZamani;
    }
    else if (*blokSayisi < MAX_BLOK)
    {
        ganttSurecleri[*blokSayisi] = surecIndeksi;
        ganttBaslangiclari[*blokSayisi] = baslangicZamani;
        ganttBitisleri[*blokSayisi] = bitisZamani;
        (*blokSayisi)++;
    }
}

/* Yazan: [Elif Ece YAMEN]
  Algoritmaların çalışma sonucunda oluşturduğu Gantt şemasını
    kullanıcıya ekranda gösterir.

    Gantt şeması, süreçlerin hangi zaman aralıklarında çalıştığını
    gösteren bir çizelgedir. */

void ganttYazdir(int ganttSurecleri[], int ganttBaslangiclari[],
                 int ganttBitisleri[], int blokSayisi)
{
    int donguSayaci;

    printf("\nGANTT SEMASI:\n");

    for (donguSayaci = 0; donguSayaci < blokSayisi; donguSayaci++)
    {
        if (ganttSurecleri[donguSayaci] == -1)
            printf("[BOS %d-%d] ",
                   ganttBaslangiclari[donguSayaci],
                   ganttBitisleri[donguSayaci]);
        else
            printf("[P%d %d-%d] ",
                   ganttSurecleri[donguSayaci] + 1,
                   ganttBaslangiclari[donguSayaci],
                   ganttBitisleri[donguSayaci]);

        if ((donguSayaci + 1) % 6 == 0)
            printf("\n");
    }

    printf("\n");
}

/* Yazan: [Zeynep Ecren AYDOĞAN]
  Her sürecin bitiş zamanını kullanarak bekleme ve dönüş
    sürelerini hesaplar. Daha sonra ortalama süreleri bulur. */
    
void sonuclariYazdir(int surecSayisi, int gelisZamanlari[],
                     int cpuSureleri[], int bitisZamanlari[],
                     double *ortalamaBekleme, double *ortalamaDonus)
{
    int donguSayaci;
    int donusSuresi, beklemeSuresi;
    int toplamBekleme = 0, toplamDonus = 0;

    printf("\nSurec\tGelis\tCPU\tBitis\tDonus\tBekleme\n");

    for (donguSayaci = 0; donguSayaci < surecSayisi; donguSayaci++)
    {
        donusSuresi = bitisZamanlari[donguSayaci]
                      - gelisZamanlari[donguSayaci];

        beklemeSuresi = donusSuresi - cpuSureleri[donguSayaci];

        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               donguSayaci + 1,
               gelisZamanlari[donguSayaci],
               cpuSureleri[donguSayaci],
               bitisZamanlari[donguSayaci],
               donusSuresi,
               beklemeSuresi);

        toplamDonus += donusSuresi;
        toplamBekleme += beklemeSuresi;
    }

    *ortalamaBekleme = (double)toplamBekleme / surecSayisi;
    *ortalamaDonus = (double)toplamDonus / surecSayisi;

    printf("\nOrtalama bekleme: %.2f\n", *ortalamaBekleme);
    printf("Ortalama donus: %.2f\n", *ortalamaDonus);
}

/* Yazan: [Elif Ece YAMEN]
  FCFS (First Come First Served) algoritmasını çalıştırır.

    FCFS, ilk gelen sürecin ilk çalıştırıldığı zamanlama yöntemidir.
    Yani işlemciye önce gelen süreç, CPU'yu önce kullanır. */
    
void fcfs(int surecSayisi, int gelisZamanlari[], int cpuSureleri[],
          int bitisZamanlari[], int ganttSurecleri[],
          int ganttBaslangiclari[], int ganttBitisleri[],
          int *blokSayisi, int *baglamDegisimiSayisi)
{
    int surecSirasi[MAX];
    int donguSayaci, ikinciDonguSayaci;
    int geciciDeger, mevcutZaman = 0;
    int oncekiSurecIndeksi = -1;

    *blokSayisi = 0;
    *baglamDegisimiSayisi = 0;

    for (donguSayaci = 0; donguSayaci < surecSayisi; donguSayaci++)
        surecSirasi[donguSayaci] = donguSayaci;

    for (donguSayaci = 0; donguSayaci < surecSayisi - 1; donguSayaci++)
    {
        for (ikinciDonguSayaci = 0;
             ikinciDonguSayaci < surecSayisi - donguSayaci - 1;
             ikinciDonguSayaci++)
        {
            if (gelisZamanlari[surecSirasi[ikinciDonguSayaci]] >
                gelisZamanlari[surecSirasi[ikinciDonguSayaci + 1]] ||
                (gelisZamanlari[surecSirasi[ikinciDonguSayaci]] ==
                 gelisZamanlari[surecSirasi[ikinciDonguSayaci + 1]] &&
                 surecSirasi[ikinciDonguSayaci] >
                 surecSirasi[ikinciDonguSayaci + 1]))
            {
                geciciDeger = surecSirasi[ikinciDonguSayaci];
                surecSirasi[ikinciDonguSayaci] =
                    surecSirasi[ikinciDonguSayaci + 1];
                surecSirasi[ikinciDonguSayaci + 1] = geciciDeger;
            }
        }
    }

    for (donguSayaci = 0; donguSayaci < surecSayisi; donguSayaci++)
    {
        int surecIndeksi = surecSirasi[donguSayaci];

        if (mevcutZaman < gelisZamanlari[surecIndeksi])
        {
            blokEkle(-1, mevcutZaman,
                     gelisZamanlari[surecIndeksi],
                     ganttSurecleri, ganttBaslangiclari, ganttBitisleri,
                     blokSayisi, baglamDegisimiSayisi,
                     &oncekiSurecIndeksi);

            mevcutZaman = gelisZamanlari[surecIndeksi];
        }

        blokEkle(surecIndeksi, mevcutZaman,
                 mevcutZaman + cpuSureleri[surecIndeksi],
                 ganttSurecleri, ganttBaslangiclari, ganttBitisleri,
                 blokSayisi, baglamDegisimiSayisi,
                 &oncekiSurecIndeksi);

        mevcutZaman += cpuSureleri[surecIndeksi];
        bitisZamanlari[surecIndeksi] = mevcutZaman;
    }
}

/* Yazan: [Zeynep Ecren AYDOĞAN]
  SJF (Shortest Job First) algoritmasını çalıştırır.

    SJF algoritmasında işlemciye hazır olan süreçler arasından
    CPU süresi en kısa olan süreç seçilir.

    Bu programda kesintisiz SJF yöntemi kullanılmıştır.
    Yani bir süreç çalışmaya başladığında işi bitene kadar
    işlemciyi kullanmaya devam eder. */
    
void sjf(int surecSayisi, int gelisZamanlari[], int cpuSureleri[],
         int bitisZamanlari[], int ganttSurecleri[],
         int ganttBaslangiclari[], int ganttBitisleri[],
         int *blokSayisi, int *baglamDegisimiSayisi)
{
    int kalanSurecler[MAX];
    int donguSayaci, secilenSurec;
    int tamamlananSurec = 0, mevcutZaman = 0;
    int oncekiSurecIndeksi = -1;

    *blokSayisi = 0;
    *baglamDegisimiSayisi = 0;

    for (donguSayaci = 0; donguSayaci < surecSayisi; donguSayaci++)
        kalanSurecler[donguSayaci] = 1;

    while (tamamlananSurec < surecSayisi)
    {
        secilenSurec = -1;

        for (donguSayaci = 0; donguSayaci < surecSayisi; donguSayaci++)
        {
            if (kalanSurecler[donguSayaci] &&
                gelisZamanlari[donguSayaci] <= mevcutZaman)
            {
                if (secilenSurec == -1 ||
                    cpuSureleri[donguSayaci] < cpuSureleri[secilenSurec] ||
                    (cpuSureleri[donguSayaci] == cpuSureleri[secilenSurec] &&
                     (gelisZamanlari[donguSayaci] <
                      gelisZamanlari[secilenSurec] ||
                      (gelisZamanlari[donguSayaci] ==
                       gelisZamanlari[secilenSurec] &&
                       donguSayaci < secilenSurec))))
                {
                    secilenSurec = donguSayaci;
                }
            }
        }

        if (secilenSurec == -1)
        {
            int enErkenSurec = -1;

            for (donguSayaci = 0; donguSayaci < surecSayisi; donguSayaci++)
            {
                if (kalanSurecler[donguSayaci] &&
                    (enErkenSurec == -1 ||
                     gelisZamanlari[donguSayaci] <
                     gelisZamanlari[enErkenSurec]))
                    enErkenSurec = donguSayaci;
            }

            blokEkle(-1, mevcutZaman,
                     gelisZamanlari[enErkenSurec],
                     ganttSurecleri, ganttBaslangiclari, ganttBitisleri,
                     blokSayisi, baglamDegisimiSayisi,
                     &oncekiSurecIndeksi);

            mevcutZaman = gelisZamanlari[enErkenSurec];
        }
        else
        {
            blokEkle(secilenSurec, mevcutZaman,
                     mevcutZaman + cpuSureleri[secilenSurec],
                     ganttSurecleri, ganttBaslangiclari, ganttBitisleri,
                     blokSayisi, baglamDegisimiSayisi,
                     &oncekiSurecIndeksi);

            mevcutZaman += cpuSureleri[secilenSurec];
            bitisZamanlari[secilenSurec] = mevcutZaman;
            kalanSurecler[secilenSurec] = 0;
            tamamlananSurec++;
        }
    }
}

/* Yazan: [Elif Ece YAMEN]
   Round Robin zamanlama algoritmasını çalıştırır.

    Round Robin algoritmasında her sürece belirli bir çalışma
    süresi verilir. Bu süre zaman dilimi olarak adlandırılır.

    Bir süreç kendisine verilen zaman dilimi içerisinde tamamlanırsa
    işlemciyi bırakır. Tamamlanamazsa kalan CPU süresi saklanır
    ve süreç daha sonra tekrar çalıştırılır. */
    
void roundRobin(int surecSayisi, int gelisZamanlari[],
                int cpuSureleri[], int zamanDilimi,
                int bitisZamanlari[], int ganttSurecleri[],
                int ganttBaslangiclari[], int ganttBitisleri[],
                int *blokSayisi, int *baglamDegisimiSayisi)
{
    int kalanSureler[MAX];
    int donguSayaci, calismaSuresi;
    int mevcutZaman = 0, tamamlananSurec = 0;
    int calisanSurecVar, enErkenSurec;
    int oncekiSurecIndeksi = -1;

    *blokSayisi = 0;
    *baglamDegisimiSayisi = 0;

    for (donguSayaci = 0; donguSayaci < surecSayisi; donguSayaci++)
        kalanSureler[donguSayaci] = cpuSureleri[donguSayaci];

    while (tamamlananSurec < surecSayisi)
    {
        calisanSurecVar = 0;

        for (donguSayaci = 0; donguSayaci < surecSayisi; donguSayaci++)
        {
            if (kalanSureler[donguSayaci] > 0 &&
                gelisZamanlari[donguSayaci] <= mevcutZaman)
            {
                calisanSurecVar = 1;

                if (kalanSureler[donguSayaci] < zamanDilimi)
                    calismaSuresi = kalanSureler[donguSayaci];
                else
                    calismaSuresi = zamanDilimi;

                blokEkle(donguSayaci, mevcutZaman,
                         mevcutZaman + calismaSuresi,
                         ganttSurecleri, ganttBaslangiclari, ganttBitisleri,
                         blokSayisi, baglamDegisimiSayisi,
                         &oncekiSurecIndeksi);

                mevcutZaman += calismaSuresi;
                kalanSureler[donguSayaci] -= calismaSuresi;

                if (kalanSureler[donguSayaci] == 0)
                {
                    bitisZamanlari[donguSayaci] = mevcutZaman;
                    tamamlananSurec++;
                }
            }
        }

        if (!calisanSurecVar && tamamlananSurec < surecSayisi)
        {
            enErkenSurec = -1;

            for (donguSayaci = 0; donguSayaci < surecSayisi; donguSayaci++)
            {
                if (kalanSureler[donguSayaci] > 0 &&
                    (enErkenSurec == -1 ||
                     gelisZamanlari[donguSayaci] <
                     gelisZamanlari[enErkenSurec]))
                    enErkenSurec = donguSayaci;
            }

            if (enErkenSurec != -1 &&
                gelisZamanlari[enErkenSurec] > mevcutZaman)
            {
                blokEkle(-1, mevcutZaman,
                         gelisZamanlari[enErkenSurec],
                         ganttSurecleri, ganttBaslangiclari, ganttBitisleri,
                         blokSayisi, baglamDegisimiSayisi,
                         &oncekiSurecIndeksi);

                mevcutZaman = gelisZamanlari[enErkenSurec];
            }
        }
    }
}

/* Yazan: [Zeynep Ecren AYDOĞAN]
   Çalıştırılan zamanlama algoritmasının sonuçlarını tek bir
    fonksiyon içerisinde ekrana yazdırır. */
    
void rapor(int surecSayisi, int gelisZamanlari[], int cpuSureleri[],
           int bitisZamanlari[], int ganttSurecleri[],
           int ganttBaslangiclari[], int ganttBitisleri[],
           int blokSayisi, int baglamDegisimiSayisi,
           double *ortalamaBekleme, double *ortalamaDonus)
{
    ganttYazdir(ganttSurecleri, ganttBaslangiclari,
                ganttBitisleri, blokSayisi);

    sonuclariYazdir(surecSayisi, gelisZamanlari, cpuSureleri,
                    bitisZamanlari, ortalamaBekleme, ortalamaDonus);

    printf("Baglam degisimi sayisi: %d\n", baglamDegisimiSayisi);
}

/* Yazan: [Zeynep Ecren AYDOĞAN ve Elif Ece YAMEN]
   AProgramın başlangıç noktasıdır.
    Programın bütün işlemlerini ve algoritmaların çalıştırılmasını yönetir ve karşılaştırır. */
    
int main(void)
{
    int surecSayisi;
    int gelisZamanlari[MAX], cpuSureleri[MAX], bitisZamanlari[MAX];

    int ganttSurecleri[MAX_BLOK];
    int ganttBaslangiclari[MAX_BLOK];
    int ganttBitisleri[MAX_BLOK];

    int blokSayisi, baglamDegisimiSayisi;
    int zamanDilimi, donguSayaci;

    double fcfsBekleme, fcfsDonus;
    double sjfBekleme, sjfDonus;
    double rrBekleme, rrDonus;

    printf("CPU ZAMANLAYICI SIMULASYONU\n");

    printf("Surec sayisini giriniz (1-20): ");
    scanf("%d", &surecSayisi);

    if (surecSayisi < 1 || surecSayisi > MAX)
    {
        printf("Hatali surec sayisi!\n");
        return 1;
    }

    for (donguSayaci = 0; donguSayaci < surecSayisi; donguSayaci++)
    {
        printf("P%d gelis zamanini giriniz: ", donguSayaci + 1);
        scanf("%d", &gelisZamanlari[donguSayaci]);

        printf("P%d CPU suresini giriniz: ", donguSayaci + 1);
        scanf("%d", &cpuSureleri[donguSayaci]);

        if (gelisZamanlari[donguSayaci] < 0 ||
            cpuSureleri[donguSayaci] <= 0)
        {
            printf("Hatali deger!\n");
            return 1;
        }
    }

    printf("Round Robin zaman dilimini giriniz: ");
    scanf("%d", &zamanDilimi);

    if (zamanDilimi <= 0)
    {
        printf("Zaman dilimi pozitif olmalidir!\n");
        return 1;
    }

    printf("\n========== FCFS ==========\n");

    fcfs(surecSayisi, gelisZamanlari, cpuSureleri, bitisZamanlari,
         ganttSurecleri, ganttBaslangiclari, ganttBitisleri,
         &blokSayisi, &baglamDegisimiSayisi);

    rapor(surecSayisi, gelisZamanlari, cpuSureleri, bitisZamanlari,
          ganttSurecleri, ganttBaslangiclari, ganttBitisleri,
          blokSayisi, baglamDegisimiSayisi,
          &fcfsBekleme, &fcfsDonus);

    printf("\n========== SJF ==========\n");

    sjf(surecSayisi, gelisZamanlari, cpuSureleri, bitisZamanlari,
        ganttSurecleri, ganttBaslangiclari, ganttBitisleri,
        &blokSayisi, &baglamDegisimiSayisi);

    rapor(surecSayisi, gelisZamanlari, cpuSureleri, bitisZamanlari,
          ganttSurecleri, ganttBaslangiclari, ganttBitisleri,
          blokSayisi, baglamDegisimiSayisi,
          &sjfBekleme, &sjfDonus);

    printf("\n========== ROUND ROBIN ==========\n");

    roundRobin(surecSayisi, gelisZamanlari, cpuSureleri, zamanDilimi,
               bitisZamanlari, ganttSurecleri,
               ganttBaslangiclari, ganttBitisleri,
               &blokSayisi, &baglamDegisimiSayisi);

    rapor(surecSayisi, gelisZamanlari, cpuSureleri, bitisZamanlari,
          ganttSurecleri, ganttBaslangiclari, ganttBitisleri,
          blokSayisi, baglamDegisimiSayisi,
          &rrBekleme, &rrDonus);

    printf("\n========== KARSILASTIRMA ==========\n");

    printf("FCFS ortalama bekleme: %.2f\n", fcfsBekleme);
    printf("SJF ortalama bekleme: %.2f\n", sjfBekleme);
    printf("Round Robin ortalama bekleme: %.2f\n", rrBekleme);

    if (fcfsBekleme <= sjfBekleme && fcfsBekleme <= rrBekleme)
        printf("En dusuk bekleme suresi: FCFS\n");
    else if (sjfBekleme <= rrBekleme)
        printf("En dusuk bekleme suresi: SJF\n");
    else
        printf("En dusuk bekleme suresi: Round Robin\n");

	return 0;
}
