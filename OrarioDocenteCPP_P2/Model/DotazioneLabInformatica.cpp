#include "DotazioneLabInformatica.h"
DotazioneLabInformatica::DotazioneLabInformatica(std::vector<std::string> postazioni, bool presenzaLIM)
    : postazioni(postazioni), presenzaLIM(presenzaLIM) {}
int DotazioneLabInformatica::GetNumeroPostazioni() {
    return static_cast<int>(postazioni.size());
}
std::string DotazioneLabInformatica::GetDescrizionePostazione(int indice) {
    if (indice < 0 || indice >= GetNumeroPostazioni()) return "";
    return postazioni[indice];
}
bool DotazioneLabInformatica::GetPresenzaLIM() { return presenzaLIM; }
