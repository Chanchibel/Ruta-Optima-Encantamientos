#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <cstdint>
#include <array>
#include <boost/container/static_vector.hpp>
#include <boost/unordered/unordered_flat_set.hpp>
#include <boost/unordered/unordered_flat_map.hpp>

namespace {
    std::vector<int> LibrosNivs = { 20, 20, 16, 12, 12, 10, 10, 10, 8, 6 };
    boost::unordered_flat_set<uint64_t> visitados;
    int COE = LibrosNivs.size() + 1;

    struct Item {
        uint32_t Id;
        uint16_t costo_base;
        uint8_t N;
        bool Herramienta;
        Item() = default;

        Item(uint32_t id, uint16_t costo, uint8_t n, bool herramienta)
            : Id(id), costo_base(costo), N(n), Herramienta(herramienta) {
        }

        bool operator>(const Item& otro) const {
            return costo_base > otro.costo_base;
        }

        bool operator==(const Item& otro) const {
            return Id == otro.Id &&
                costo_base == otro.costo_base &&
                N == otro.N &&
                Herramienta == otro.Herramienta;
        }
    };

    struct Inventario {
        boost::container::static_vector<Item, 24> items_rest;
        int costo_g;
        int estimacion_h;
        int costo_f;

        Inventario(boost::container::static_vector<Item, 24> objs, int g, int h)
            : items_rest(std::move(objs)), costo_g(g), estimacion_h(h) {
            costo_f = costo_g + estimacion_h;
        }

        bool operator>(const Inventario& otro) const {
            int p1 = COE - items_rest.size();
            int p2 = COE - otro.items_rest.size();
            if (p1 != p2) { return p1 < p2; }
            return costo_f > otro.costo_f;
        }
    };

    using FirmaInventario = boost::container::static_vector<Item, 24>;

    struct HashFirma {
        std::size_t operator()(const FirmaInventario& items) const {
            std::size_t seed = 0;
            for (const auto& item : items) {
                boost::hash_combine(seed, item.Id);
                boost::hash_combine(seed, item.N);
                boost::hash_combine(seed, item.costo_base);
            }
            return seed;
        }
    };

    int puntos_por_nivel(int nivel) {
        if (nivel < 17) {
            return (nivel * nivel) + (6 * nivel);
        }
        if (nivel < 32) {
            return (5 * nivel * nivel - 81 * nivel + 720) / 2;
        }
        else {
            return (9 * nivel * nivel - 325 * nivel + 4440) / 2;
        }
    }

    int aproximacion(const boost::container::static_vector<Item, 24>& Items) {
        int suma = 0;
        for (size_t i = 0; i < Items.size(); i++) {
            if (Items[i].Herramienta) continue;
            suma += puntos_por_nivel(Items[i].costo_base);
        }
        return suma;
    }

    inline uint64_t empacar(uint16_t a, uint8_t b, uint16_t c, uint8_t d) {
        return (static_cast<uint64_t>(a) << 32) |
            (static_cast<uint64_t>(b) << 24) |
            (static_cast<uint64_t>(c) << 8) |
            static_cast<uint64_t>(d);
    }

    boost::container::static_vector<Item, 24> Objetos;
    int MejorCosto = std::numeric_limits<int>::max();
    boost::unordered_flat_map<FirmaInventario, int, HashFirma> inventarios_visitados;
    int contador;
}

int main1() {

    visitados.reserve(2000);
    constexpr std::size_t LimiteIV = 15000000;
    inventarios_visitados.reserve(LimiteIV);
    Objetos.push_back(Item(1, 0, 0, true));

    for (size_t i = 0; i < LibrosNivs.size(); ++i) {
        int mask = 1 << (i + 1);
        Objetos.push_back(Item(mask, LibrosNivs[i], 0, false));
    }

    int costo_ap = aproximacion(Objetos);

    std::priority_queue<Inventario, std::vector<Inventario>, std::greater<Inventario>> cola;
    Inventario Inv(Objetos, 0, costo_ap);
    cola.push(Inv);
    std::cout << Inv.costo_f << '\n';

    while (!cola.empty()) {
        contador += 1;
        Inventario actual = cola.top();
        cola.pop();

        if (actual.costo_f >= MejorCosto) continue;

        boost::container::static_vector<Item, 24> ObjetosAct = actual.items_rest;
        size_t n_items = ObjetosAct.size();

        if (n_items == 1) {
            MejorCosto = actual.costo_f;
            std::cout << "¡Éxito! Mejor costo total de XP encontrado: " << actual.costo_g << '\n';
        }

        for (size_t i = 0; i < n_items; i++) {
            for (size_t j = i + 1; j < n_items; j++) {
                const Item& item1 = ObjetosAct[i];
                const Item& item2 = ObjetosAct[j];

                uint64_t clave;

                if (item1 > item2) {
                    clave = empacar(item1.costo_base, item1.N, item2.costo_base, item2.N);
                }
                else {
                    clave = empacar(item2.costo_base, item2.N, item1.costo_base, item1.N);
                }

                if (visitados.contains(clave)) { continue; }
                visitados.insert(clave);

                int Nivelescb = (item1.Herramienta || (!item2.Herramienta && item1.costo_base > item2.costo_base))
                    ? item2.costo_base
                    : item1.costo_base;
                int NivelesN = ((1 << item1.N) - 1) + ((1 << item2.N) - 1);
                int xp_gastada = puntos_por_nivel(NivelesN + Nivelescb);
                int NCostoG = actual.costo_g + xp_gastada;

                Item Nitem(item1.Id | item2.Id, item1.costo_base + item2.costo_base, std::max(item1.N, item2.N) + 1, item1.Herramienta || item2.Herramienta);

                size_t min_idx = std::min(i, j);
                size_t max_idx = std::max(i, j);

                boost::container::static_vector<Item, 24> NObjetos;
                NObjetos.push_back(Nitem);

                NObjetos.insert(NObjetos.end(), ObjetosAct.begin(), ObjetosAct.begin() + min_idx);
                NObjetos.insert(NObjetos.end(), ObjetosAct.begin() + min_idx + 1, ObjetosAct.begin() + max_idx);
                NObjetos.insert(NObjetos.end(), ObjetosAct.begin() + max_idx + 1, ObjetosAct.begin() + n_items);

                int NAprox = aproximacion(Objetos);
                int NCostoF = NCostoG + NAprox;

                if (NCostoF >= MejorCosto) continue;

                FirmaInventario firma = NObjetos;
                std::sort(firma.begin(), firma.end(), [](const Item& a, const Item& b) {
                    if (a.Id != b.Id) return a.Id < b.Id;
                    if (a.N != b.N) return a.N < b.N;
                    return a.costo_base < b.costo_base;
                    });

                auto it = inventarios_visitados.find(firma);
                if (it != inventarios_visitados.end() && it->second <= NCostoG) {
                    continue; // Ya exploramos este inventario con un costo_g menor o igual
                }

                if (inventarios_visitados.size() >= LimiteIV) { inventarios_visitados.clear(); }

                inventarios_visitados[firma] = NCostoG;
                cola.emplace(std::move(NObjetos), NCostoG, NAprox);
            }
        }
        visitados.clear();
    }
    std::cout << contador << '\n';
    inventarios_visitados.clear();
    inventarios_visitados.rehash(0);
    visitados.rehash(0);

    return 0;
}
