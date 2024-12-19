#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int JML_KOLOM = 6;

struct Tugas
{
    int idTugas;         // Nomor unik untuk setiap tugas
    string namaTugas;    // Nama tugas
    string mataKuliah;   // Mata kuliah
    string tenggatWaktu; // Format: DD/MM/YYYY
    int prioritas;       // 1 = tinggi, 2 = sedang, 3 = rendah
    bool selesai;        // true jika selesai, false jika belum
};

/* Begin Fungsi Pembantu */

// Fungsi untuk mengubah string prioritas ("Tinggi", "Sedang", "Rendah") menjadi nilai integer (1, 2, 3)
// Dijelaskan oleh: Vasya
int prioritasTugasKeInt(string prioritas)
{
    if (prioritas == "Tinggi")
    {
        return 1;
    }
    else if (prioritas == "Sedang")
    {
        return 2;
    }
    else if (prioritas == "Rendah")
    {
        return 3;
    }
    else
    {
        return -1; // Nilai untuk input tidak valid
    }
}

// Fungsi untuk mengubah nilai integer prioritas menjadi string prioritas
// Dijelaskan oleh: Vasya
string intKePrioritasTugas(int prioritas)
{
    string daftarPrioritas[3] = {"Tinggi", "Sedang", "Rendah"};
    if (prioritas < 1 || prioritas > 3)
        return "Tidak Ada";
    return daftarPrioritas[prioritas - 1];
}

// Fungsi untuk mengubah string status tugas menjadi boolean
// Dijelaskan oleh: Vasya
bool statusTugasKeBool(string status)
{
    return (status == "Selesai");
}

// Fungsi untuk mengubah boolean status tugas menjadi string
// Dijelaskan oleh: Vasya
string boolKeStatusTugas(bool selesai)
{
    if (selesai)
        return "Selesai";
    return "Belum Selesai";
}

// Fungsi untuk mengecek apakah string hanya berisi angka (numerik)
// Dijelaskan oleh: Bima
bool cekNumerik(string str)
{
    // Jika string kosong, tidak valid
    if (str.empty())
        return false;

    // Memeriksa setiap karakter dalam string
    for (int i = 0; i < str.length(); i++)
    {
        // Karakter harus berada dalam rentang '0' hingga '9'
        if (str[i] < '0' || str[i] > '9')
        {
            return false;
        }
    }

    return true;
}

// Fungsi untuk memisahkan string berdasarkan karakter pemisah '|'
// Contoh: "1|b|c|d|Tinggi|Belum Selesai" diubah menjadi {"1", "b", "c", "d", "Tinggi", "Belum Selesai"}
// Dijelaskan oleh: Bima
void lineKeArray(string line, string *arr)
{
    int insertIndex = 0;
    string temp;

    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == '|')
        {
            arr[insertIndex] = temp;
            temp = "";
            insertIndex++;
        }
        else
        {
            temp += line[i];
        }
    }

    arr[insertIndex] = temp;
}

// Fungsi untuk mengubah array string menjadi struct Tugas
// Dijelaskan oleh: Hilmi
void arrayKeStruct(string *arr, Tugas &tugas)
{
    tugas.idTugas = stoi(arr[0]); // stoi = string to integer
    tugas.namaTugas = arr[1];
    tugas.mataKuliah = arr[2];
    tugas.tenggatWaktu = arr[3];
    tugas.prioritas = prioritasTugasKeInt(arr[4]);
    tugas.selesai = statusTugasKeBool(arr[5]);
}

// Fungsi untuk menghitung jumlah baris dalam file
// Dijelaskan oleh: Hilmi
int hitungBarisFile(const string namaFile)
{
    ifstream fileData(namaFile);
    int jumlahBaris = 0;
    string line;

    // Menghitung jumlah baris dalam file
    while (getline(fileData, line))
    {
        jumlahBaris++;
    }

    fileData.close();
    return jumlahBaris;
}

// Fungsi untuk mengecek apakah string adalah format tanggal valid (DD/MM/YYYY)
// Dijelaskan oleh: Bima
bool formatTanggalValid(string tgl)
{
    // Panjang string harus 10 (DD/MM/YYYY)
    if (tgl.length() != 10)
        return false;

    // Memeriksa apakah karakter pada posisi 3 dan 6 adalah '/'
    if (tgl[2] != '/' || tgl[5] != '/')
        return false;

    // Menyimpan string hari, bulan, dan tahun
    string hariStr = tgl.substr(0, 2);  // DD
    string bulanStr = tgl.substr(3, 2); // MM
    string tahunStr = tgl.substr(6, 4); // YYYY

    // Menyimpan boolean true apabila isinya numerik, false jika bukan numerik
    bool hariNumerik = cekNumerik(hariStr);
    bool bulanNumerik = cekNumerik(bulanStr);
    bool tahunNumerik = cekNumerik(tahunStr);

    // Mengecek apakah semuanya numerik
    if (!hariNumerik || !bulanNumerik || !tahunNumerik)
        return false;

    // Mengubah bagian string menjadi integer
    int hari = stoi(hariStr);   // DD
    int bulan = stoi(bulanStr); // MM
    int tahun = stoi(tahunStr); // YYYY

    // Memeriksa validitas bulan
    if (bulan < 1 || bulan > 12)
        return false;

    // Memeriksa validitas hari berdasarkan bulan
    int hariMax;
    if (bulan == 1 || bulan == 3 || bulan == 5 || bulan == 7 || bulan == 8 || bulan == 10 || bulan == 12)
    {
        hariMax = 31; // Bulan dengan 31 hari
    }
    else if (bulan == 4 || bulan == 6 || bulan == 9 || bulan == 11)
    {
        hariMax = 30; // Bulan dengan 30 hari
    }
    else if (bulan == 2)
    {
        // Februari: Periksa tahun kabisat
        if ((tahun % 4 == 0 && tahun % 100 != 0) || (tahun % 400 == 0))
        {
            hariMax = 29; // Tahun kabisat
        }
        else
        {
            hariMax = 28; // Bukan tahun kabisat
        }
    }

    // Memeriksa validitas hari
    if (hari < 1 || hari > hariMax)
        return false;

    return true;
}

// Fungsi untuk mengecek apakah pilihan berada dalam rentang opsi yang valid
// Dijelaskan oleh: Hilmi
bool cekPilihan(string pilihan, int opsiMin, int opsiMax)
{
    if (!cekNumerik(pilihan))
        return false;

    int pil = stoi(pilihan);

    return (pil >= opsiMin && pil <= opsiMax);
}

// Fungsi untuk mencari index sebuah tugas berdasarkan ID tugas
// Dijelaskan oleh: Bima
int cariIndexTugas(Tugas *daftarTugas, int jumlahTugas, int idTugas)
{
    for (int i = 0; i < jumlahTugas; i++)
    {
        if (daftarTugas[i].idTugas == idTugas)
        {
            return i; // Kembalikan indeks jika ditemukan
        }
    }
    return -1; // Kembalikan -1 jika tidak ditemukan
}

// Fungsi untuk menambahkan spasi agar teks sesuai dengan lebar kolom tabel
// Dijelaskan oleh: Bima
string tambahkanSpasi(string teks, int lebar)
{
    string hasil = teks;
    while (hasil.length() < lebar)
    {
        hasil += " ";
    }
    return hasil;
}

// Fungsi untuk mengecek apakah jumlah karakter "|" sesuai
// Dijelaskan oleh: Bima
bool lineValid(string line)
{
    int separatorCount = 0;

    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == '|')
            separatorCount++;
    }

    return (separatorCount == JML_KOLOM - 1);
}

// Fungsi untuk memuat data pada file menjadi array of struct Tugas
// Dijelaskan oleh: Bima
Tugas *muatData(bool *dataKorup)
{
    // Membuka file dan deklarasi variabel
    ifstream fileData("data.txt");
    int panjangArr = hitungBarisFile("data.txt");
    Tugas *daftarTugas;
    string line;

    // Array 2D yang akan diisi daftar tugas
    string daftarTugasStr[panjangArr][JML_KOLOM];

    // Mengisi array daftarTugasStr dengan tugas
    int lineIndex = 0;
    while (getline(fileData, line))
    {
        // Apabila jumlah karakter "|" tidak sesuai maka dataKorup menjadi true
        if (!lineValid(line))
        {
            *dataKorup = true;
            return nullptr; // pointer kosong
        }
        lineKeArray(line, daftarTugasStr[lineIndex]);
        lineIndex++;
    }

    // Array yang akan diisi struct Tugas
    daftarTugas = new Tugas[panjangArr];

    // Mengisi array daftarTugas dari array daftarTugasStr yang telah diubah menjadi array of struct Tugas
    for (int i = 0; i < panjangArr; i++)
    {
        arrayKeStruct(daftarTugasStr[i], daftarTugas[i]);
    }

    // Menutup file
    fileData.close();

    return daftarTugas;
}

// Fungsi untuk menyimpan semua tugas ke dalam file
// Dijelaskan oleh: Vasya
void simpanData(Tugas *daftarTugas, int jumlahTugas)
{
    ofstream fileData("data.txt");
    string data, temp;

    for (int i = 0; i < jumlahTugas; i++)
    {
        temp = "";
        temp += to_string(daftarTugas[i].idTugas) + "|";
        temp += daftarTugas[i].namaTugas + "|";
        temp += daftarTugas[i].mataKuliah + "|";
        temp += daftarTugas[i].tenggatWaktu + "|";
        temp += intKePrioritasTugas(daftarTugas[i].prioritas) + "|";
        temp += boolKeStatusTugas(daftarTugas[i].selesai) + "\n";
        data += temp;
    }

    fileData << data;
}

// Fungsi untuk mengecek apakah suatu string legal atau tidak terdapat karakter "|"
// Dijelaskan oleh: Bima
bool inputLegal(string input)
{
    return (input.find('|') == string::npos);
}

// Fungsi untuk menyortir array Tugas berdasarkan ID
// Dijelaskan oleh: Bima
void sortirTugasBerdasarkanID(Tugas *daftarTugas, int jumlahTugas)
{
    // Bubble Sort untuk menyortir berdasarkan ID
    for (int i = 0; i < jumlahTugas - 1; i++)
    {
        for (int j = 0; j < jumlahTugas - i - 1; j++)
        {
            if (daftarTugas[j].idTugas > daftarTugas[j + 1].idTugas)
            {
                // Tukar elemen jika ID j lebih besar daripada j+1
                Tugas temp = daftarTugas[j];
                daftarTugas[j] = daftarTugas[j + 1];
                daftarTugas[j + 1] = temp;
            }
        }
    }
}

// Fungsi untuk memecah string tanggal menjadi hari, bulan, dan tahun
// Dijelaskan oleh: Hilmi
void pecahTanggal(const string tanggal, int &hari, int &bulan, int &tahun)
{
    hari = stoi(tanggal.substr(0, 2));  // Ambil DD
    bulan = stoi(tanggal.substr(3, 2)); // Ambil MM
    tahun = stoi(tanggal.substr(6, 4)); // Ambil YYYY
}

// Fungsi untuk membandingkan dua tanggal dalam format DD/MM/YYYY
// Dijelaskan oleh: Hilmi
bool tglLebihAwal(const string tanggal1, const string tanggal2)
{
    int hari1, bulan1, tahun1;
    int hari2, bulan2, tahun2;

    // Pecah kedua tanggal menjadi komponen hari, bulan, dan tahun
    pecahTanggal(tanggal1, hari1, bulan1, tahun1);
    pecahTanggal(tanggal2, hari2, bulan2, tahun2);

    // Perbandingan hierarkis
    if (tahun1 != tahun2)
    {
        return tahun1 < tahun2; // Tahun lebih kecil lebih awal
    }
    else if (bulan1 != bulan2)
    {
        return bulan1 < bulan2; // Bulan lebih kecil lebih awal
    }
    else
    {
        return hari1 < hari2; // Hari lebih kecil lebih awal
    }
}

// Fungsi untuk menyortir array Tugas berdasarkan tenggat waktu
// Dijelaskan oleh: Bima
void sortirTugasBerdasarkanTanggal(Tugas *daftarTugas, int jumlahTugas)
{
    // Bubble Sort untuk menyortir berdasarkan tanggal
    for (int i = 0; i < jumlahTugas - 1; i++)
    {
        for (int j = 0; j < jumlahTugas - i - 1; j++)
        {
            if (!tglLebihAwal(daftarTugas[j].tenggatWaktu, daftarTugas[j + 1].tenggatWaktu))
            {
                // Tukar elemen jika tanggal j lebih lambat daripada j+1
                Tugas temp = daftarTugas[j];
                daftarTugas[j] = daftarTugas[j + 1];
                daftarTugas[j + 1] = temp;
            }
        }
    }
}

// Fungsi untuk menyortir array Tugas berdasarkan prioritas
// Dijelaskan oleh: Bima
void sortirTugasBerdasarkanPrioritas(Tugas *daftarTugas, int jumlahTugas)
{
    // Bubble Sort untuk menyortir berdasarkan prioritas
    for (int i = 0; i < jumlahTugas - 1; i++)
    {
        for (int j = 0; j < jumlahTugas - i - 1; j++)
        {
            if (daftarTugas[j].prioritas > daftarTugas[j + 1].prioritas)
            {
                // Tukar elemen jika prioritas j lebih besar daripada j+1
                Tugas temp = daftarTugas[j];
                daftarTugas[j] = daftarTugas[j + 1];
                daftarTugas[j + 1] = temp;
            }
        }
    }
}

// Fungsi untuk sortir urutan tampilan tugas
// Dijelaskan oleh: Vasya
void sortirTugas(Tugas *daftarTugas, int jumlahTugas, int sortir)
{
    switch (sortir)
    {
    case 1:
        sortirTugasBerdasarkanID(daftarTugas, jumlahTugas);
        break;
    case 2:
        sortirTugasBerdasarkanTanggal(daftarTugas, jumlahTugas);
        break;
    case 3:
        sortirTugasBerdasarkanPrioritas(daftarTugas, jumlahTugas);
        break;
    }
}

// Fungsi untuk menampilkan tugas dalam bentuk tabel
// Dijelaskan oleh: Hilmi
void tabelTugas(Tugas *daftarTugas, int jumlahTugas, int sortir = 1)
{
    // Sortir urutan tugas sebelum ditampilkan
    sortirTugas(daftarTugas, jumlahTugas, sortir);

    // Header tabel
    cout << tambahkanSpasi("ID", 5)
         << tambahkanSpasi("Nama Tugas", 35)
         << tambahkanSpasi("Mata Kuliah", 25)
         << tambahkanSpasi("Tenggat Waktu", 20)
         << tambahkanSpasi("Prioritas", 15)
         << tambahkanSpasi("Status", 20) << endl;

    cout << string(115, '-') << endl;

    // Isi tabel
    for (int i = 0; i < jumlahTugas; i++)
    {
        cout << tambahkanSpasi(to_string(daftarTugas[i].idTugas), 5)
             << tambahkanSpasi(daftarTugas[i].namaTugas, 35)
             << tambahkanSpasi(daftarTugas[i].mataKuliah, 25)
             << tambahkanSpasi(daftarTugas[i].tenggatWaktu, 20)
             << tambahkanSpasi(intKePrioritasTugas(daftarTugas[i].prioritas), 15)
             << tambahkanSpasi(boolKeStatusTugas(daftarTugas[i].selesai), 20) << endl;
    }

    cout << endl;
}

// Fungsi untuk menampilkan satu tugas dalam bentuk tabel
// Dijelaskan oleh: Hilmi
void tabelSatuTugas(Tugas *daftarTugas, int indexTugas)
{
    int jumlahTugas = 1;
    Tugas daftarTugasBaru[jumlahTugas] = {
        {
            daftarTugas[indexTugas].idTugas,
            daftarTugas[indexTugas].namaTugas,
            daftarTugas[indexTugas].mataKuliah,
            daftarTugas[indexTugas].tenggatWaktu,
            daftarTugas[indexTugas].prioritas,
            daftarTugas[indexTugas].selesai,
        }};

    tabelTugas(daftarTugasBaru, jumlahTugas);
}

// Fungsi untuk menampilkan antarmuka
// Dijelaskan oleh: Vasya
void menuUtama()
{
    cout << endl
         << "########################################" << endl
         << "###     PENGELOLAAN TUGAS KULIAH     ###" << endl
         << "########################################" << endl
         << endl
         << "1. Tambah Tugas" << endl
         << "2. Lihat Daftar Tugas" << endl
         << "3. Edit Tugas" << endl
         << "4. Hapus Tugas" << endl
         << "5. Tandai Tugas Selesai" << endl
         << "6. Simpan dan Keluar" << endl
         << endl
         << "Pilih opsi (1-6): ";
}

// Fungsi untuk menampilkan antarmuka
// Dijelaskan oleh: Vasya
void menuSortirTugas(int sortir)
{
    cout << "Sortir tugas berdasarkan:" << endl
         << "1. ID (default)" << endl
         << "2. Tenggat Waktu" << endl
         << "3. Prioritas" << endl
         << "Pilihan saat ini: " << sortir << endl
         << endl
         << "Pilih 0 untuk kembali ke menu utama" << endl
         << endl
         << "Pilih opsi (1/2/3/0): ";
}

/* End Fungsi Pembantu */

/* Begin Fungsi Utama */

// Fungsi untuk menambahkan tugas ke dalam file
// Dijelaskan oleh: Bima
void tambahTugas()
{
    string idTugas, namaTugas, matkul, tenggat, prioritas, selesai;

    // input nama tugas
    while (true)
    {
        cout << "Masukkan nama tugas: ";
        getline(cin, namaTugas);

        // Apabila input kosong maka continue statement akan dijalankan dan loop mulai lagi dari awal
        if (empty(namaTugas))
        {
            cout << "Teks tidak boleh kosong!" << endl;
            continue;
        }
        // Apabila terdapat karakter "|" maka continue statement akan dijalankan dan loop mulai lagi dari awal
        if (!inputLegal(namaTugas))
        {
            cout << "Teks tidak boleh mengandung karakter \"|\"" << endl;
            continue;
        }
        // Break statement untuk menghentikan loop
        break;
    }

    // input mata kuliah
    while (true)
    {
        cout << "Masukkan mata kuliah: ";
        getline(cin, matkul);

        // Cek apakah input kosong
        if (empty(matkul))
        {
            cout << "Teks tidak boleh kosong!" << endl;
            continue;
        }
        if (!inputLegal(matkul))
        {
            cout << "Teks tidak boleh mengandung karakter \"|\"" << endl;
            continue;
        }
        break;
    }

    // input tenggat waktu
    while (true)
    {
        cout << "Masukkan tenggat waktu (DD/MM/YYYY): ";
        getline(cin, tenggat);

        // Apabila format tanggal tidak valid maka continue statement akan dijalankan dan loop mulai lagi dari awal
        if (!formatTanggalValid(tenggat))
        {
            cout << "Format tanggal tidak valid!" << endl;
            continue;
        }
        break;
    }

    // input prioritas
    while (true)
    {
        cout << "Masukkan prioritas (1:tinggi/2:sedang/3:rendah): ";
        getline(cin, prioritas);

        // Deklarasi rentang opsi yang dapat di-input (1-3)
        int opsiMin = 1, opsiMax = 3;

        if (!cekPilihan(prioritas, opsiMin, opsiMax))
        {
            cout << "Pilihan tidak valid!" << endl;
            continue;
        }

        prioritas = intKePrioritasTugas(stoi(prioritas));
        break;
    }

    idTugas = to_string(hitungBarisFile("data.txt") + 1);
    selesai = "Belum Selesai";

    // Menggabungkan semua input menjadi satu string
    string hasilGabung = idTugas + "|" + namaTugas + "|" + matkul + "|" + tenggat + "|" + prioritas + "|" + selesai;

    // Menyimpan hasil ke file
    ofstream fileTugas("data.txt", ios::app);
    fileTugas << hasilGabung << endl;
    fileTugas.close();

    cout << endl
         << "Tugas berhasil ditambahkan!" << endl
         << endl;
}

// Fungsi untuk menampilkan daftar tugas dalam format tabel
// Dijelaskan oleh: Vasya & Bima
void lihatDaftarTugas(Tugas *daftarTugas)
{
    int jumlahTugas = hitungBarisFile("data.txt");
    int pilihan, opsiMin = 0, opsiMax = 3;
    string pilihanStr;
    bool pilihanValid;

    /*
    sortir = 1 --> Sortir berdasarkan ID (default)
    sortir = 2 --> Sortir berdasarkan tenggat waktu
    sortir = 3 --> Sortir berdasarkan prioritas
     */
    int sortir = 1;
    if (jumlahTugas > 0)
    {
        while (true)
        {
            tabelTugas(daftarTugas, jumlahTugas, sortir);
            menuSortirTugas(sortir);
            getline(cin, pilihanStr);
            pilihanValid = cekPilihan(pilihanStr, opsiMin, opsiMax);

            if (!pilihanValid)
            {
                cout << "Nomor pilihan " << pilihanStr << " tidak ada pada opsi, mohon pilih ulang!" << endl;
                continue;
            }

            pilihan = stoi(pilihanStr);

            if (pilihan == 0)
                break;

            sortir = pilihan;
        }
    }
    else
    {
        cout << endl
             << "Daftar tugas masih kosong!" << endl
             << endl;
    }
}

// Dijelaskan oleh: Hilmi
void menuUbahIsiTugas()
{
    cout << "Pilih yang ingin diubah: " << endl
         << "1. Nama Tugas" << endl
         << "2. Mata Kuliah" << endl
         << "3. Tenggat Waktu" << endl
         << "4. Prioritas" << endl
         << endl
         << "Pilih 0 untuk membatalkan dan kembali ke menu utama" << endl
         << endl
         << "Pilih opsi (1/2/3/4/0): ";
}

// Fungsi untuk mengubah isi tugas berdasarkan index
// Dijelaskan oleh: Bima
void ubahIsiTugas(Tugas *daftarTugas, int indexTugas, int pilihan)
{
    string daftarInstruksi[4] = {
        "Masukkan nama tugas baru",
        "Masukkan mata kuliah baru",
        "Masukkan tenggat waktu baru (DD/MM/YYYY)",
        "Masukkan prioritas baru (1:tinggi/2:sedang/3:rendah)",
    };
    string input;
    bool loop = true;

    while (loop)
    {
        cout << daftarInstruksi[pilihan - 1];
        cout << " (kosongi untuk membatalkan): ";
        getline(cin, input);
        cout << endl;

        if (empty(input))
        {
            cout << "Edit tugas berhasil dibatalkan!" << endl;
            break;
        };

        switch (pilihan)
        {
        case 1:
            if (!inputLegal(input))
            {
                cout << "Teks tidak boleh mengandung karakter \"|\"" << endl;
                break;
            }
            daftarTugas[indexTugas].namaTugas = input;
            loop = false;
            break;
        case 2:
            if (!inputLegal(input))
            {
                cout << "Teks tidak boleh mengandung karakter \"|\"" << endl;
                break;
            }
            daftarTugas[indexTugas].mataKuliah = input;
            loop = false;
            break;
        case 3:
            if (!formatTanggalValid(input))
            {
                cout << "Format tanggal tidak valid!" << endl;
                break;
            }
            daftarTugas[indexTugas].tenggatWaktu = input;
            loop = false;
            break;
        case 4:
            int opsiMin = 1, opsiMax = 3;
            if (!cekPilihan(input, opsiMin, opsiMax))
            {
                cout << "Pilihan " << input << " tidak ada pada opsi!" << endl;
                break;
            }
            daftarTugas[indexTugas].prioritas = stoi(input);
            loop = false;
            break;
        }
    }
}

// Fungsi untuk mengedit tugas
// Dijelaskan oleh: Bima
void editTugas(Tugas *daftarTugas)
{
    bool idTugasValid, pilihanValid;
    int idTugas, indexTugas, pilihan, opsiPilihanMin, opsiPilihanMax, jumlahTugas = hitungBarisFile("data.txt");
    string idTugasStr, pilihanStr;

    if (jumlahTugas > 0)
    {
        while (true)
        {
            cout << "Masukkan ID tugas: ";
            getline(cin, idTugasStr);
            idTugasValid = cekNumerik(idTugasStr);

            if (!idTugasValid)
            {
                cout << "ID tugas tidak valid, mohon masukkan ulang!" << endl;
                continue;
            }

            idTugas = stoi(idTugasStr);
            indexTugas = cariIndexTugas(daftarTugas, jumlahTugas, idTugas);

            if (indexTugas == -1)
            {
                cout << "Tugas dengan ID " << idTugas << " tidak ditemukan!" << endl;
                continue;
            }

            break;
        }

        tabelSatuTugas(daftarTugas, indexTugas);

        while (true)
        {
            menuUbahIsiTugas();
            getline(cin, pilihanStr);
            opsiPilihanMin = 0;
            opsiPilihanMax = 4;
            pilihanValid = cekPilihan(pilihanStr, opsiPilihanMin, opsiPilihanMax);

            if (!pilihanValid)
            {
                cout << "Pilihan " << pilihanStr << " tidak ada pada opsi!" << endl;
                continue;
            }

            pilihan = stoi(pilihanStr);
            break;
        }

        if (pilihan != 0)
        {
            ubahIsiTugas(daftarTugas, indexTugas, pilihan);
            simpanData(daftarTugas, jumlahTugas);
            cout << "Tugas dengan ID " << idTugas << " berhasil diedit!" << endl;
        }
    }
    else
    {
        cout << endl
             << "Daftar tugas masih kosong" << endl
             << endl;
    }
}

// Fungsi untuk menghapus elemen dari array berdasarkan indeks
// Dijelaskan oleh: Bima
void hapusTugasByIndex(Tugas *daftarTugas, int &jumlahTugas, int index)
{
    // Geser elemen setelah indeks ke depan agar elemen pada indeks tertimpa
    for (int i = index; i < jumlahTugas - 1; i++)
    {
        daftarTugas[i] = daftarTugas[i + 1];
    }

    // Kurangi panjang array karena satu tugas sudah ditimpa/dihapus
    jumlahTugas--;

    // Update ID tugas agar kembali berurutan
    for (int i = 0; i < jumlahTugas; i++)
    {
        daftarTugas[i].idTugas = i + 1;
    }
}

// Fungsi untuk menghapus tugas
// Dijelaskan oleh: Bima
void hapusTugas(Tugas *daftarTugas)
{
    string idTugasStr, konfirmasi;
    int idTugas, indexTugas = -1, jumlahTugas = hitungBarisFile("data.txt");

    if (jumlahTugas > 0)
    {
        while (indexTugas == -1)
        {
            cout << "Masukkan ID tugas: ";
            getline(cin, idTugasStr);

            if (!cekNumerik(idTugasStr))
            {
                cout << "ID tugas tidak valid!" << endl;
                continue;
            }

            idTugas = stoi(idTugasStr);
            indexTugas = cariIndexTugas(daftarTugas, jumlahTugas, idTugas);

            if (indexTugas == -1)
                cout << "Tugas dengan ID " << idTugas << " tidak ditemukan!" << endl;
        }

        cout << "Apakah Anda yakin ingin menghapus tugas dengan ID " << idTugas << "?" << endl;
        cout << "Ketik YA bila yakin: ";
        getline(cin, konfirmasi);

        if (konfirmasi == "YA")
        {
            hapusTugasByIndex(daftarTugas, jumlahTugas, indexTugas);
            simpanData(daftarTugas, jumlahTugas);
            cout << "Tugas berhasil dihapus!" << endl;
        }
        else
        {
            cout << "Penghapusan tugas berhasil dibatalkan!" << endl;
        }
    }
    else
    {
        cout << "Daftar tugas masih kosong!" << endl;
    }
}

// Fungsi untuk menandai tugas selesai/belum selesai
// Dijelaskan oleh: Bima
void tandaiSelesai(Tugas *daftarTugas)
{
    int idTugas, indexTugas, jumlahTugas = hitungBarisFile("data.txt");
    bool selesai;
    string idTugasStr;

    if (jumlahTugas > 0)
    {
        while (true)
        {
            cout << "Masukkan ID tugas: ";
            getline(cin, idTugasStr);
            if (!cekNumerik(idTugasStr))
            {
                cout << "ID tugas tidak valid!" << endl;
                continue;
            }
            idTugas = stoi(idTugasStr);
            indexTugas = cariIndexTugas(daftarTugas, jumlahTugas, idTugas);

            if (indexTugas == -1)
            {
                cout << "Tugas dengan ID " << idTugas << " tidak ditemukan!" << endl;
                continue;
            }
            break;
        }
        selesai = !(daftarTugas[indexTugas].selesai);
        daftarTugas[indexTugas].selesai = selesai;
        simpanData(daftarTugas, jumlahTugas);

        cout << "Status tugas dengan ID " << idTugas << " berhasil diubah menjadi " << boolKeStatusTugas(selesai) << "!" << endl;
    }
    else
    {
        cout << endl
             << "Daftar tugas masih kosong" << endl
             << endl;
    }
}

// Dijelaskan oleh: Vasya
void simpanDanKeluar()
{
    cout << "Berhasil menyimpan dan keluar!" << endl;
    cout << "Terima kasih telah menggunakan program Pengelolaan Tugas Kuliah!" << endl;
}

// Dijelaskan oleh: Vasya
void pesanPilihanError(int pilihan)
{
    cout << "Error: Nomor " << pilihan << " tidak ada pada opsi, mohon pilih ulang!" << endl;
}

// Dijelaskan oleh: Bima
void mulai()
{
    bool dataKorup;
    Tugas *daftarTugas = muatData(&dataKorup);
    int pilihan;
    string pilihanStr;

    // Pastikan data tidak korup
    if (!dataKorup)
    {
        while (true)
        {
            menuUtama();
            getline(cin, pilihanStr);
            if (!cekNumerik(pilihanStr))
            {
                cout << "Pilihan harus berupa bilangan bulat!" << endl
                     << endl;
                continue;
            }
            pilihan = stoi(pilihanStr); // stoi = string to integer
            break;
        }

        switch (pilihan)
        {
        case 1:
            tambahTugas();
            break;
        case 2:
            lihatDaftarTugas(daftarTugas);
            break;
        case 3:
            editTugas(daftarTugas);
            break;
        case 4:
            hapusTugas(daftarTugas);
            break;
        case 5:
            tandaiSelesai(daftarTugas);
            break;
        case 6:
            simpanDanKeluar();
            break;
        default:
            pesanPilihanError(pilihan);
            break;
        }

        delete[] daftarTugas; // Menghapus dynamic variable daftarTugas setelah tidak digunakan agar tidak terjadi memory leak

        if (pilihan != 6)
            mulai();
    }
    else
    {
        cout << endl
             << "Data korup, mohon benahi secara manual!" << endl
             << endl;
    }
}

/* End Fungsi Utama */

int main()
{
    mulai();
    return 0;
}
