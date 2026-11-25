#include <algorithm>
#include <format>
#include <iostream>
#include <print>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace carnet {

constexpr double noteMin = 0.0;
constexpr double noteMax = 20.0;
constexpr double seuilValidation = 10.0;

enum class Ordre { Croissant, Decroissant };

struct Statistiques {
    double minimum{};
    double maximum{};
    double moyenne{};
    int nbValidees{};
};

[[nodiscard]] int lireEntier(std::string_view invite, int min, int max) {
    while (std::cin) {
        std::cout << invite;
        std::string ligne;
        if (!std::getline(std::cin, ligne)) {
            break;
        }
        std::istringstream flux(ligne);
        int valeur{};
        char reste{};
        if (!(flux >> valeur) || (flux >> reste)) {
            std::cout << "Saisie invalide : entrez un entier.\n";
        } else if (valeur < min || valeur > max) {
            std::cout << "La valeur doit etre comprise entre " << min
                      << " et " << max << ".\n";
        } else {
            return valeur;
        }
    }
    return min;
}

[[nodiscard]] double lireNote(std::string_view invite) {
    while (std::cin) {
        std::cout << invite;
        std::string ligne;
        if (!std::getline(std::cin, ligne)) {
            break;
        }
        for (char& c : ligne) {
            if (c == ',') {
                c = '.';
            }
        }
        std::istringstream flux(ligne);
        double valeur{};
        char reste{};
        if (!(flux >> valeur) || (flux >> reste)) {
            std::cout << "Saisie invalide : entrez un nombre.\n";
        } else if (valeur < noteMin || valeur > noteMax) {
            std::cout << "La note doit etre comprise entre " << noteMin
                      << " et " << noteMax << ".\n";
        } else {
            return valeur;
        }
    }
    return noteMin;
}

[[nodiscard]] Statistiques calculerStatistiques(const std::vector<double>& notes) {
    double mini = notes.front();
    double maxi = notes.front();
    double somme = 0.0;
    int validees = 0;

    for (double n : notes) {
        if (n < mini) {
            mini = n;
        }
        if (n > maxi) {
            maxi = n;
        }
        somme += n;
        if (n >= seuilValidation) {
            ++validees;
        }
    }

    const double moy = somme / static_cast<double>(notes.size());
    return Statistiques{mini, maxi, moy, validees};
}

void appliquerBonus(std::vector<double>& notes, double bonus) {
    std::ranges::for_each(notes, [bonus](double& n) {
        n = std::clamp(n + bonus, noteMin, noteMax);
    });
}

void trier(std::vector<double>& notes, Ordre ordre) {
    if (ordre == Ordre::Croissant) {
        std::ranges::sort(notes, [](double a, double b) { return a < b; });
    } else {
        std::ranges::sort(notes, [](double a, double b) { return a > b; });
    }
}

void afficherNotes(const std::vector<double>& notes) {
    if (notes.empty()) {
        std::println("Aucune note.");
        return;
    }
    for (std::size_t i = 0; i < notes.size(); ++i) {
        std::println("  {}. {:.2f}", i + 1, notes[i]);
    }
}

void ajouter(std::vector<double>& notes) {
    notes.push_back(lireNote("Note a ajouter : "));
}

void modifier(std::vector<double>& notes) {
    if (notes.empty()) {
        std::println("Aucune note a modifier.");
        return;
    }
    afficherNotes(notes);
    const int rang = lireEntier("Numero de la note : ", 1, static_cast<int>(notes.size()));
    notes[static_cast<std::size_t>(rang - 1)] = lireNote("Nouvelle valeur : ");
}

void supprimer(std::vector<double>& notes) {
    if (notes.empty()) {
        std::println("Aucune note a supprimer.");
        return;
    }
    afficherNotes(notes);
    const int rang = lireEntier("Numero de la note : ", 1, static_cast<int>(notes.size()));
    notes.erase(notes.begin() + (rang - 1));
}

void trierMenu(std::vector<double>& notes) {
    if (notes.empty()) {
        std::println("Aucune note a trier.");
        return;
    }
    const int sens = lireEntier("1. Croissant  2. Decroissant : ", 1, 2);
    trier(notes, sens == 1 ? Ordre::Croissant : Ordre::Decroissant);
}

void afficherStatistiques(const std::vector<double>& notes) {
    if (notes.empty()) {
        std::println("Aucune note.");
        return;
    }
    const auto [mini, maxi, moyenne, validees] = calculerStatistiques(notes);
    std::println("Min : {:.2f} | Max : {:.2f} | Moyenne : {:.2f} | Validees : {} / {}",
                 mini, maxi, moyenne, validees, notes.size());
}

void bonusMenu(std::vector<double>& notes) {
    if (notes.empty()) {
        std::println("Aucune note.");
        return;
    }
    const double bonus = lireNote("Bonus : ");
    appliquerBonus(notes, bonus);
}

[[nodiscard]] int afficherMenu() {
    std::println("\n=== Carnet de notes ===");
    std::println("1. Ajouter une note");
    std::println("2. Afficher les notes");
    std::println("3. Modifier une note");
    std::println("4. Supprimer une note");
    std::println("5. Trier les notes (croissant / decroissant)");
    std::println("6. Statistiques");
    std::println("7. Appliquer un bonus");
    std::println("0. Quitter");
    return lireEntier("Votre choix : ", 0, 7);
}

}  // namespace carnet

int main() {
    std::vector<double> notes;
    int choix{};
    do {
        choix = carnet::afficherMenu();
        switch (choix) {
            case 1: carnet::ajouter(notes); break;
            case 2: carnet::afficherNotes(notes); break;
            case 3: carnet::modifier(notes); break;
            case 4: carnet::supprimer(notes); break;
            case 5: carnet::trierMenu(notes); break;
            case 6: carnet::afficherStatistiques(notes); break;
            case 7: carnet::bonusMenu(notes); break;
            case 0: break;
            default: break;
        }
    } while (choix != 0);

    return 0;
}
