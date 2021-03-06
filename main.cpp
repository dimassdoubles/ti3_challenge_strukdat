#include <iostream>
using namespace std;

// Konstanta nama setiap role
const string STRIKER = "Striker";
const string MIDFIELDER = "Midfielder";
const string DEFENDER = "Defender";
const string GOALKEEPER = "Goalkeeper";
const int KUOTA_HAPUS = 3;

struct Player {
    string nama_player;
    
    Player *prev;
    Player *next = nullptr;
};

struct Role {
    int kapasitas = 0;
    int isi = 0;
    string nama_role;

    Role *next = nullptr;
    
    Player *pemain_pertama = nullptr;
};

struct List {
    int kuota_hapus = KUOTA_HAPUS;
    int kartu_merah = 0;
    bool drop_out = false;
    string pemain_kartu_merah[4];
    string pemain_akhir_pertandingan[7];
    Role *head = nullptr;
};

Player *new_player;

void tambah_role(List *l, string nama, int kapasitas) {
    bool is_empty = l->head == NULL;
    Role *temp =  new Role;
    temp->nama_role = nama;
    temp->kapasitas = kapasitas;
    Role *cursor;

    if (is_empty) {
        l->head = temp;
    } else {
        cursor = l->head;
        while (cursor->next != NULL) {
            cursor = cursor->next;
        }
        cursor->next = temp;
    }
}

void cetak_role(List* l) {
    Role *cursor = new Role;
    cursor = l->head;
    if (cursor != NULL) {
        while (cursor->next != NULL) {
            cout << cursor->nama_role << " --> ";
            cursor = cursor->next;
        }
        cout << cursor->nama_role << endl;
    }
}

void buat_formasi(List *l, int kapasitas[3]) {
    tambah_role(l, STRIKER, kapasitas[0]);
    tambah_role(l, MIDFIELDER, kapasitas[1]);
    tambah_role(l, DEFENDER, kapasitas[2]);

    // kapasitas goalkeeper selalu 1
    tambah_role(l, GOALKEEPER, 1);
}

void input_formasi(List *l) {
    int kapasitas[3];
    int kuota_pemain = 10;
    int striker;
    int midfielder;
    int defender;
    cout << "Sisa kuota = 10, Masukan jumlah Striker    : "; cin >> striker;
    kuota_pemain -= striker;
    cout << "Sisa kuota =  " << kuota_pemain << ", Masukan jumlah Midfielder : "; cin >> midfielder;
    kuota_pemain -= midfielder;
    cout << "Sisa kuota =  " << kuota_pemain << ", Masukan jumlah Defender   : "; cin >> defender;
    kuota_pemain -= defender;

    if (kuota_pemain == 0 && striker != 0 && midfielder != 0 && defender != 0) {
        kapasitas[0] = striker;
        kapasitas[1] = midfielder;
        kapasitas[2] = defender;
        buat_formasi(l, kapasitas);
    } else {
        cout << "\nFormasi tidak valid, silahkan ulangi lagi\n" << endl;
        input_formasi(l);
    }
}

bool is_no_player(Role *r) {
    if (r->pemain_pertama == NULL) {
        return true;
    } 
    return false;
}

bool is_role_full(Role *r) {
    if (r->isi >= r->kapasitas) {
        return true;
    }
    return false;
}

Role* pilih_role(List *l, string nama_role) {
    if (nama_role == STRIKER) {
        return l->head;
    } else if (nama_role == MIDFIELDER) {
        return (l->head)->next;
    } else if (nama_role == DEFENDER) {
        return ((l->head)->next)->next;
    } else if (nama_role == GOALKEEPER) {
        return (((l->head)->next)->next)->next;
    }
    return nullptr; 
}

bool player_ada_di_role_v1(List *l, string nama_role, string nama_player) {
    Role *role = pilih_role(l, nama_role);
    Player *cursor = role->pemain_pertama;
    if (!is_no_player(role)) {
        if (cursor->nama_player == nama_player) {
            return true;
        }

        while (cursor->next != NULL) {
            cursor = cursor->next;
            if (cursor->nama_player == nama_player) {
                return true;
            }
        }
    }
    return false;
}

bool player_ada_di_role_v2(Role *r, string nama_player) {
    Player *cursor = r->pemain_pertama;
    if (!is_no_player(r)) {
        if (cursor->nama_player == nama_player) {
            cout << "Pemain ditemukan, " << cursor->nama_player << " ada di Role " << r->nama_role << endl;
            return true;
        }
        while (cursor->next != NULL) {
            cursor = cursor->next;
            if (cursor->nama_player == nama_player) {
                cout << "Pemain ditemukan, " << cursor->nama_player << " ada di Role " << r->nama_role << endl;
                return true;
            }
        }
    }
    return false;
}

bool is_player_sudah_ada(List *l, string nama_player) {
    bool player_ada_dilapangan = player_ada_di_role_v1(l, STRIKER, nama_player) || player_ada_di_role_v1(l, MIDFIELDER, nama_player) || player_ada_di_role_v1(l, DEFENDER, nama_player) || player_ada_di_role_v1(l, GOALKEEPER, nama_player);
    bool player_ada_didaftar_merah = (nama_player == l->pemain_kartu_merah[0]) || (nama_player == l->pemain_kartu_merah[1]) || (nama_player == l->pemain_kartu_merah[2]) || (nama_player == l->pemain_kartu_merah[3]);
    if (player_ada_dilapangan) {
        return true;
    } else if (player_ada_didaftar_merah) {
        cout << "Pemain ditemukan, " << nama_player << " ada di daftar pemain yang terkena kartu merah" << endl;
        return true;
    }
    return false;
}

void alokasi_player_baru(string nama_player) {
    Player *temp = new Player;
    temp->nama_player = nama_player;

    new_player = temp;
}

void tampil_role_v1(List*l, string nama_role) {
    Role *role = pilih_role(l, nama_role);
    Player *cursor = role->pemain_pertama;
    int i = 1;

    if (is_no_player(role)) {
        cout << "Role " << nama_role << " kosong" << endl;
    } else {
        cout << "Daftar pemain di role " << nama_role << ": " << endl;
        cout << i << ". " << cursor->nama_player << endl;
        while (cursor->next != NULL) {
            cursor = cursor->next;
            i ++;
            cout << i << ". " << cursor->nama_player << endl;
        }
    }
}

int tampil_role_v2(Role *r, int urutan_mulai) {
    Player *cursor = r->pemain_pertama;
    if (!is_no_player(r)) {
        cout << urutan_mulai << ". " << cursor->nama_player << " (" << r->nama_role << ")" << endl;
        while (cursor->next != NULL) {
            cursor = cursor->next;
            urutan_mulai ++;
            cout << urutan_mulai << ". " << cursor->nama_player << " (" << r->nama_role << ")" << endl;
        }
        return urutan_mulai + 1;
    } 

    return urutan_mulai;
}

void tambah_player(List *l, string nama_role) {
    Role *role = pilih_role(l, nama_role);
    Player *cursor = role->pemain_pertama;
    string nama_player;

    if (is_role_full(role)) {
        cout << "Maaf, role " << nama_role << " sudah penuh" << endl;
        tampil_role_v1(l, nama_role);
    } else {
        cout << "Masukan nama player: "; cin >> nama_player;
        if (is_player_sudah_ada(l, nama_player)) {
            cout << "Maaf, " << nama_player << " sudah terdaftar di list pemain" << endl;
        } else {
            alokasi_player_baru(nama_player);
            if (is_no_player(role)) {
                new_player->prev = nullptr;
                role->pemain_pertama = new_player;
            } else {
                while (cursor->next != NULL) {
                    cursor = cursor->next;
                }
                new_player->prev = cursor;
                cursor->next = new_player; 
            }

            cout << nama_player << " berhasil ditambahkan ke " << nama_role << endl;
            role->isi ++;
            tampil_role_v1(l, nama_role);
        }
    }
}

void hapus_player_v1(List *l, string nama_role) {
    bool player_pertama = false;
    bool player_terakhir = false;
    Role *role = pilih_role(l, nama_role);
    Player *cursor = role->pemain_pertama;
    Player *deleted = nullptr;
    string nama_player;
    
    if (l->kuota_hapus <= 0) {
        cout << "Maaf, kuota hapus sudah habis. Anda tidak dapat menghapus pemain" << endl;
    } else if (is_no_player(role)) {
        cout << "Role " << nama_role << " kosong" << endl;
    } else {
        tampil_role_v1(l, nama_role);
        cout << "Masukan nama player: "; cin >> nama_player;
        if (cursor->nama_player == nama_player) {
            player_pertama = true;
            deleted = cursor;
            if (deleted->next == NULL) {
                // cout << "#cek 2" << endl;
                player_terakhir = true;
            }
        } else {
            cursor = cursor->next;
            while (cursor->next != NULL) {
                if (cursor->nama_player == nama_player) {
                    deleted = cursor;
                }
                cursor = cursor->next;
            }
            if (cursor->nama_player == nama_player) {
                    // cout << "#cek 3" << endl;
                    deleted = cursor;
                    player_terakhir = true;
                }
        }

        if (deleted == NULL) {
            cout << nama_player << " tidak ada di role " << nama_role << endl;
        } else if (player_pertama && player_terakhir) {
            // cout << "#cek 4" << endl;
            role->pemain_pertama = nullptr;
            delete deleted;
            cout << nama_player << " berhasil dihapus" << endl;
            role->isi --;
            l->kuota_hapus --;
            tampil_role_v1(l, nama_role);
        } else if (player_pertama) {
            // cout << "#cek 5" << endl;
            (deleted->next)->prev = nullptr;
            role->pemain_pertama = deleted->next;
            delete deleted;
            cout << nama_player << " berhasil dihapus" << endl;
            role->isi --;
            l->kuota_hapus --;
            tampil_role_v1(l, nama_role);
        } else if (player_terakhir) {
            // cout << "#cek 6" << endl;
            (deleted->prev)->next = nullptr;
            delete deleted;
            cout << nama_player << " berhasil dihapus" << endl;
            role->isi --;
            l->kuota_hapus --;
            tampil_role_v1(l, nama_role);
        } else {
            // cout << "#cek 1" << endl;
            (deleted->prev)->next = deleted->next;
            (deleted->next)->prev = deleted->prev;
            // cout << deleted->prev->nama_player << endl;
            delete deleted;
            cout << nama_player << " berhasil dihapus" << endl;
            role->isi --;
            l->kuota_hapus --;
            tampil_role_v1(l, nama_role);
        }
    }
}

void hapus_player_v2(Player *p) {
    Player *deleted = p;
    delete deleted;
}

void hapus_role(Role *r) {
    Player *cursor = r->pemain_pertama;
    Player *deleted = cursor;
    if (!is_no_player(r)) {
        while (cursor->next != NULL) {
            cursor = cursor->next;
            hapus_player_v2(deleted);
            deleted = cursor;
        }
        hapus_player_v2(deleted);
    }
    r->isi = 0;
    r->pemain_pertama = nullptr;
}

void subtitusi_player(List *l, string nama_role) {
    Role *role = pilih_role(l, nama_role);
    Player *cursor = role->pemain_pertama;
    Player *deleted = nullptr;
    bool is_pemain_pertama_diganti = false;
    bool is_pemain_terakhir_diganti = false;
    string nama_diganti;
    string nama_pengganti;

    tampil_role_v1(l, nama_role);
    cout << "Masukan nama player yang ingin diganti: "; cin >> nama_diganti;
    if (is_no_player(role)) {
        cout << "Role " << nama_role << " kosong" << endl;
    } else {
        if (cursor->nama_player == nama_diganti) {
            is_pemain_pertama_diganti = true;
            deleted = cursor;
            if (deleted->next == NULL) {
                is_pemain_terakhir_diganti = true;
            }
        } else {
            cursor = cursor->next;
            while (cursor->next != NULL) {
                if (cursor->nama_player == nama_diganti) {
                    deleted = cursor;
                }
                cursor = cursor->next;
            }
            if (cursor->nama_player == nama_diganti) {
                    is_pemain_terakhir_diganti = true;
                    deleted = cursor;
                }
        }

        if (deleted == NULL) {
            cout << "Maaf, " << nama_diganti << " tidak ada di role " << nama_role << endl;
        } else {
            cout << "Masukan nama player pengganti: "; cin >> nama_pengganti;
            if (is_player_sudah_ada(l, nama_pengganti)) {
                cout << "Maaf, " << nama_pengganti << " sudah ada di list pemain" << endl;
            } else {
                alokasi_player_baru(nama_pengganti);
                if (is_pemain_pertama_diganti && is_pemain_terakhir_diganti) {
                    new_player->prev = nullptr;
                    new_player->next = nullptr;
                    role->pemain_pertama = new_player;
                } else if (is_pemain_pertama_diganti) {
                    new_player->prev = nullptr;
                    new_player->next = deleted->next;
                    (deleted->next)->prev = new_player;
                    role->pemain_pertama = new_player;
                } else if (is_pemain_terakhir_diganti) {
                    new_player->prev = deleted->prev;
                    (deleted->prev)->next = new_player;
                } else {
                    new_player->prev = deleted->prev;
                    new_player->next = deleted->next;
                    (deleted->prev)->next = new_player;
                    (deleted->next)->prev = new_player;
                }

                delete deleted;

                cout << "Berhasil mengganti " << nama_diganti << " dengan " << nama_pengganti << endl;
            }
            tampil_role_v1(l, nama_role);
        }
    }
}

// FUNGSI WAJIB ------------------------------------------
void tambah_striker(List *l) {
    tambah_player(l, STRIKER);
}

void tambah_midfielder(List *l) {
    tambah_player(l, MIDFIELDER);
}

void tambah_defender(List *l) {
    tambah_player(l, DEFENDER);
}

void hapus_striker(List *l) {
    hapus_player_v1(l, STRIKER);
}

void hapus_midfielder(List *l) {
    hapus_player_v1(l, MIDFIELDER);
}

void hapus_defender(List *l) {
    hapus_player_v1(l, DEFENDER);
}

void subtitusi_striker(List *l) {
    subtitusi_player(l, STRIKER);
}

void subtitusi_midfielder(List* l) {
    subtitusi_player(l, MIDFIELDER);
}

void subtitusi_defender(List *l) {
    subtitusi_player(l, DEFENDER);
}

void tampil_striker(List *l) {
    tampil_role_v1(l, STRIKER);
}

void tampil_midfielder(List *l) {
    tampil_role_v1(l, MIDFIELDER);
}

void tampil_defender(List *l) {
    tampil_role_v1(l, DEFENDER);
}

void tampil_pemain(List *l) {
    int i = 1;
    Role *striker = l->head;
    Role *midfielder = (l->head)->next;
    Role *defender = ((l->head)->next)->next;
    Role *goalkeeper = (((l->head)->next)->next)->next;

    bool is_empty = is_no_player(striker) && is_no_player(midfielder) && is_no_player(defender) && is_no_player(goalkeeper);

    if (is_empty) {
        cout << "Belum ada pemain di list pemain" << endl;
    } else {
        cout << "Daftar nama-nama pemain: " << endl;
        i = tampil_role_v2(striker, i);
        i = tampil_role_v2(midfielder, i);
        i = tampil_role_v2(defender, i);
        tampil_role_v2(goalkeeper, i);
    }
}

void hapus_pemain(List *l) {
    Role *striker = l->head;
    Role *midfielder  = (l->head)->next;
    Role *defender = ((l->head)->next)->next;
    Role *goalkeeper = (((l->head)->next)->next)->next;

    hapus_role(striker);
    hapus_role(midfielder);
    hapus_role(defender);
    hapus_role(goalkeeper);
    
    cout << "Semua pemain sudah dihapus" << endl;
    l->kuota_hapus = KUOTA_HAPUS;
}

void cari_pemain(List *l) {
    Role *striker = pilih_role(l, STRIKER);
    Role *midfielder = pilih_role(l, MIDFIELDER);
    Role *defender = pilih_role(l, DEFENDER);
    Role *goalkeeper = pilih_role(l, GOALKEEPER);
    string nama_player;

    cout << "Masukan nama player: "; cin >> nama_player;

    if (player_ada_di_role_v2(striker, nama_player)) {

    } else if (player_ada_di_role_v2(midfielder, nama_player)) {

    } else if (player_ada_di_role_v2(defender, nama_player)) {

    } else if (player_ada_di_role_v2(goalkeeper, nama_player)) {

    } else {
        cout << nama_player << " tidak ditemukan di role manapun" << endl;
    }
    
}

// MODIFIKASI DAN VARIASI ---------------------------------------------------------

int hapus_role_v2(Role *r, int indeks_mulai, string pemain_akhir_pertandingan[7]) {
    Player *cursor = r->pemain_pertama;
    Player *deleted = cursor;
    if (!is_no_player(r)) {
        while (cursor->next != NULL) {
            cursor = cursor->next;
            pemain_akhir_pertandingan[indeks_mulai] = deleted->nama_player + " (" + r->nama_role + ")";
            indeks_mulai ++;
            delete deleted;
            deleted = cursor;
        }
        pemain_akhir_pertandingan[indeks_mulai] = deleted->nama_player + " (" + r->nama_role + ")";
        indeks_mulai ++;
        delete deleted;
        r->isi = 0;
        r->pemain_pertama = nullptr;
        return indeks_mulai;
    }

    return indeks_mulai;
}

void hapus_pemain_v2(List *l) {
    int indeks_mulai = 0;
    Role *striker = pilih_role(l, STRIKER);
    Role *midfielder = pilih_role(l, MIDFIELDER);
    Role *defender = pilih_role(l, DEFENDER);
    Role *goalkeeper = pilih_role(l, GOALKEEPER);

    indeks_mulai = hapus_role_v2(striker, indeks_mulai, l->pemain_akhir_pertandingan);
    indeks_mulai = hapus_role_v2(midfielder, indeks_mulai, l->pemain_akhir_pertandingan);
    indeks_mulai = hapus_role_v2(defender, indeks_mulai, l->pemain_akhir_pertandingan);
    hapus_role_v2(goalkeeper, indeks_mulai, l->pemain_akhir_pertandingan);
}

void tambah_list_merah(List *l, Player *p) {
    int i = 1;
    if (l->pemain_kartu_merah[0] == "") {
        l->pemain_kartu_merah[0] = p->nama_player;
    } else {
        while (l->pemain_kartu_merah[i] != "") {
            i ++;
        }
        l->pemain_kartu_merah[i] = p->nama_player;
    }

    delete p;
}

void tampil_list_merah(List *l) {
    int i = 1;
    if (l->pemain_kartu_merah[0] == "") {
        cout << "Belum ada pemain yang terkena kartu merah" << endl;
    } else {
        cout << "Daftar pemain yang terkena kartu merah: " << endl;
        cout << "1. " << l->pemain_kartu_merah[0] << endl;
        while (l->pemain_kartu_merah[i] != "") {
            cout << i+1 << ". " << l->pemain_kartu_merah[i] << endl;
            i++;
        }
    }
}

void tampil_statistik(List *l) {
    int i = 0;
    cout << "Kartu merah : " << l->kartu_merah << endl;
    if (l->drop_out) {
        cout << "Daftar pemain yang bertahan sampai akhir pertandingan: " << endl;

        while (l->pemain_akhir_pertandingan[i] != "") {
            cout << i+1 << ". " << l->pemain_akhir_pertandingan[i] << endl;
            i++;
        }
    } else {
        tampil_pemain(l);
    }
}

void tambah_goalkeeper(List *l) {
    tambah_player(l, GOALKEEPER);
}

bool ganti_goalkeeper(Role *striker, Role *goalkeeper) {
        bool is_pengganti_player_pertama = false;
        bool is_pengganti_player_terakhir = false;
        Player *cursor;
        Player *player_pengganti = nullptr;
        string nama_pengganti;

        cout << "Masukan nama player pengganti: "; cin >> nama_pengganti;
        cursor = striker->pemain_pertama;
        if (cursor->nama_player == nama_pengganti) {
            is_pengganti_player_pertama = true;
            player_pengganti = cursor;
            if (cursor->next == NULL) {
                // cout << "#Cek 1" << endl;
                is_pengganti_player_terakhir = true;
            }
        } else {
            cursor = cursor->next;
            while (cursor->next != NULL) {
                if (cursor->nama_player == nama_pengganti) {
                    player_pengganti = cursor;
                }
                cursor = cursor->next;
            }
            if (cursor->nama_player == nama_pengganti) {
                is_pengganti_player_terakhir = true;
                player_pengganti = cursor;
            }
        }

        if (player_pengganti == NULL) {
            cout << "Maaf, " << nama_pengganti << " tidak ditemukan di role Striker" << endl;
        } else {
            if (is_pengganti_player_pertama && is_pengganti_player_terakhir) {
                striker->pemain_pertama = nullptr;
            } else if(is_pengganti_player_pertama) {
                // cout << "#cek 2" << endl;
                striker->pemain_pertama = player_pengganti->next;
                (player_pengganti->next)->prev = nullptr;

            } else if (is_pengganti_player_terakhir) {
                (player_pengganti->prev)->next = nullptr;
            } else {
                (player_pengganti->prev)->next = player_pengganti->next;
                (player_pengganti->next)->prev = player_pengganti->prev;
            }

            player_pengganti->prev = nullptr;
            player_pengganti->next = nullptr;
            goalkeeper->pemain_pertama = player_pengganti;

            cout << "Berhasil memindahkan " << nama_pengganti << " ke role Goalkeeper" << endl;
            goalkeeper->isi ++;
            striker->isi --;
            return false;
        }


        return true;
}


void kartu_merah(List *l) {
    Role *striker = pilih_role(l, STRIKER);
    Role *midfielder = pilih_role(l, MIDFIELDER);
    Role *defender = pilih_role(l, DEFENDER);
    Role *goalkeeper = pilih_role(l, GOALKEEPER);
    Player *cursor;
    Player *deleted = nullptr;
    Role *role_player;
    string nama_player;
    bool belum_ketemu = true;
    bool berhasil_kartu_merah = true;
    bool is_empty = is_no_player(striker) && is_no_player(midfielder) && is_no_player(defender) && is_no_player(goalkeeper);

    tampil_pemain(l);

    if (!is_empty) {
        // meminta input nama player yang akan diberi kartu merah
        cout << "Masukan nama player: "; cin >> nama_player;

        // mencari player yang akan diberi kartu merah
        if (belum_ketemu && !is_no_player(striker)) {
            // cari di striker
            // cout << "#cek 1";
            cursor = striker->pemain_pertama;
            if (cursor->nama_player == nama_player) {
                deleted = cursor;
                role_player = striker;
                belum_ketemu = false;
            } else {
                while (cursor->next != NULL) {
                    cursor = cursor->next;
                    if (cursor->nama_player == nama_player) {
                        deleted = cursor;
                        role_player = striker;
                        belum_ketemu = false;
                        break;
                    }
                }
            }
        } 
        if (belum_ketemu && !is_no_player(midfielder)) {
            // cari di midfielder
            // cout << " #cek 2";
            cursor = midfielder->pemain_pertama;
            if (cursor->nama_player == nama_player) {
                deleted = cursor;
                role_player = midfielder;
                belum_ketemu = false;
            } else {
                while (cursor->next != NULL) {
                    cursor = cursor->next;
                    if (cursor->nama_player == nama_player) {
                        deleted = cursor;
                        role_player = midfielder;
                        belum_ketemu = false;
                        break;
                    }
                }
            }
        } 
        if (belum_ketemu && !is_no_player(defender)) {
            // cari di defender
            // cout << " #cek 3";
            cursor = defender->pemain_pertama;
            if (cursor->nama_player == nama_player) {
                deleted = cursor;
                role_player = defender;
                belum_ketemu = false;
            } else {
                while (cursor->next != NULL) {
                    cursor = cursor->next;
                    if (cursor->nama_player == nama_player) {
                        deleted = cursor;
                        role_player = defender;
                        belum_ketemu = false;
                        break;
                    }
                }
            }
        } 
        if (belum_ketemu && !is_no_player(goalkeeper)) {
            // cari di goalkeeper
            // cout << " #cek 4";
            cursor = goalkeeper->pemain_pertama;
            if (cursor->nama_player == nama_player) {
                deleted = cursor;
                role_player = goalkeeper;
                belum_ketemu = false;
            } else {
                while (cursor->next != NULL) {
                    cursor = cursor->next;
                    if (cursor->nama_player == nama_player) {
                        deleted = cursor;
                        role_player = goalkeeper;
                        belum_ketemu = false;
                        break;
                    }
                }
            }
        } 
        if (belum_ketemu) {
            cout << "Player tidak ditemukan" << endl;
            berhasil_kartu_merah = false;
        }

        if (deleted != NULL) {
            // ketika keeper yang kena kartu merah
            if (role_player == goalkeeper) {
                if (l->kuota_hapus < 0) {
                    // tidak ada perintah, lewati
                }
                else if(!is_no_player(striker)) {
                    while(ganti_goalkeeper(striker, goalkeeper));
                } 
                else {
                    cout << "Kartu merah dibatalkan, tidak ada striker untuk menggantikan keeper" << endl;
                    berhasil_kartu_merah = false;
                }
            } 
            // ketika player yang dihapus adalah player pertama dan terakhir
            else if (deleted->next == deleted->prev) {
                role_player->pemain_pertama = nullptr;
            }
            // ketika player yang dihapus adalah player pertama
            else if (deleted->prev == NULL) {
                role_player->pemain_pertama = deleted->next;
                (deleted->next)->prev = nullptr;
                role_player->pemain_pertama = deleted->next;
            }
            // ketika player yang dihapus adalah player terakhir
            else if (deleted->next == NULL) {
                (deleted->prev)->next = nullptr;
            }
            // ketika player yang dihapus ada ditengah
            else {
                (deleted->prev)->next = deleted->next;
                (deleted->next)->prev = deleted->prev;
            }

        }

        if (berhasil_kartu_merah) {
            l->kartu_merah ++;
            l->kuota_hapus --;
            tambah_list_merah(l, deleted);
            cout << nama_player << " mendapatkan kartu merah" << endl;
        }

        if (l->kuota_hapus < 0) {
            cout << "Drop out, tim gugur." << endl;
            hapus_pemain_v2(l);
            l->drop_out = true;
        }
        
        // cout << "#cek 1" << endl;
    }

}

// MENU ---------------------------------------------------------------------------

bool pilih_menu(List *l) {
    int pilihan;
    cout << "\nMasukan pilihan (1 - 19): "; cin >> pilihan;

    if (pilihan == 1) {
        cout << "TAMBAH STRIKER -------------" << endl;
        tambah_striker(l);
    } else if(pilihan == 2) {
        cout << "TAMBAH STRIKER -------------" << endl;
        tambah_midfielder(l);
    } else if(pilihan == 3) {
        cout << "TAMBAH DEFENDER ------------" << endl;
        tambah_defender(l);
    } else if(pilihan == 4) {
        cout << "HAPUS STRIKER --------------" << endl;
        hapus_striker(l);
    } else if(pilihan == 5) {
        cout << "HAPUS MIDFIELDER -----------" << endl;
        hapus_midfielder(l);
    } else if(pilihan == 6) {
        cout << "HAPUS DEFENDER -------------" << endl;
        hapus_defender(l);
    } else if(pilihan == 7) {
        cout << "SUBTITUSI STRIKER ----------" << endl;
        subtitusi_striker(l);
    } else if(pilihan == 8) {
        cout << "SUBTITUSI MIDFIELDER -------" << endl;
        subtitusi_midfielder(l);
    } else if(pilihan == 9) {
        cout << "SUBTITUSI DEFENDER ---------" << endl;
        subtitusi_defender(l);
    } else if(pilihan == 10) {
        cout << "TAMPIL STRIKER -------------" << endl;
        tampil_striker(l);
    } else if(pilihan == 11) {
        cout << "TAMPIL MIDFIELDER ----------" << endl;
        tampil_midfielder(l);
    } else if(pilihan == 12) {
        cout << "TAMPIL DEFENDER ------------" << endl;
        tampil_defender(l);
    } else if(pilihan == 13) {
        cout << "TAMPIL PEMAIN --------------" << endl;
        tampil_pemain(l);
    } else if(pilihan == 14) {
        cout << "HAPUS PEMAIN ---------------" << endl;
        hapus_pemain(l);
    } else if(pilihan == 15) {
        cout << "CARI PEMAIN ----------------" << endl;
        cari_pemain(l);
    } else if (pilihan == 16) {
        cout << "TAMBAH GOALKEEPER ----------" << endl;
        tambah_goalkeeper(l);
    } else if (pilihan == 17) {
        cout << "KARTU MERAH ----------------" << endl;
        kartu_merah(l);
    } else if (pilihan == 18) {
        cout << "TAMPI STATISTIK ------------" << endl;
        tampil_statistik(l);
    } else if (pilihan == 19) {
        cout << "TAMPIL LIST MERAH ----------" << endl;
        tampil_list_merah(l);
    } else {
        return false;
    }
    return true;
}

bool pilih_menu_drop_out(List *l) {
    int pilihan;

    cout << "\nMasukan pilihan (1 - 3): "; cin >> pilihan;

    if (pilihan == 1) {
        cout << "TAMPIL STATISTIK -----------" << endl;
        tampil_statistik(l);
    } else if (pilihan == 2) {
        cout << "TAMPIL LIST MERAH ----------" << endl;
        tampil_list_merah(l);
    } else if (pilihan == 3) {
        cout << "TAMPIL PEMAIN --------------" << endl;
        tampil_pemain(l);
    } else {
        return false;
    }
    return true;
}

int main() {
    List *strategi = new List;
    bool lanjut = true;

    cout << "INPUT FORMASI --------------" << endl;
    input_formasi(strategi);
    // cetak_role(strategi);
    cout << "\nDAFTAR MENU ----------------" << endl;
    cout << " 1. Tambah Striker\n 2. Tambah Midfielder\n 3. Tambah Defender" << endl;
    cout << " 4. Hapus Striker\n 5. Hapus Midfielder\n 6. Hapus Defender" << endl;
    cout << " 7. Subtitusi Striker\n 8. Subtitusi Midfielder\n 9. Subtitusi Defender" << endl;
    cout << "10. Tampil Striker\n11. Tampil Midfielder\n12. Tampil Defender" << endl;
    cout << "13. Tampil Pemain\n14. Hapus Pemain\n15. Cari Pemain" << endl;

    // modifikasi dan variasi
    cout << "16. Tambah Goalkeeper" << endl;
    cout << "17. Kartu Merah" << endl;
    cout << "18. Tampil Statistik" << endl;
    cout << "19. Tampil List Merah" << endl;


    while (lanjut) {
        lanjut = pilih_menu(strategi);
        if (strategi->drop_out) {
            cout << "\n----PERTANDINGAN SELESAI----" << endl;
            lanjut = true;

            cout << "\nDAFTAR MENU DROP OUT -------" << endl;
            cout << " 1. Tampil Statistik" << endl;
            cout << " 2. Tampil List Merah" << endl;
            cout << " 3. Tampil Pemain" << endl;

            while (lanjut) {
                lanjut = pilih_menu_drop_out(strategi);
            }

        }
    }

    cout << "Program selesai" << endl;

    return 0;
}
