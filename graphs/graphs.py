import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path



files = ["experiment_1.csv", "experiment_2.csv", "experiment_3.csv", "experiment_4.csv"]

for file in files:
    df = pd.read_csv(file)
    time = df["time"]
    n_label = Path(file).stem

    es_acumulado = df["aceptados"].is_monotonic_increasing

    if es_acumulado:
        aceptados_bin = df["aceptados"].diff().fillna(df["aceptados"])
        tasa_bin = df["rechazados_tasa"].diff().fillna(df["rechazados_tasa"])
        lleno_bin = df["rechazados_lleno"].diff().fillna(df["rechazados_lleno"])
    else:
        aceptados_bin = df["aceptados"]
        tasa_bin = df["rechazados_tasa"]
        lleno_bin = df["rechazados_lleno"]

    fig, (ax_estado, ax_eventos) = plt.subplots(
        2, 1, figsize=(11, 7), sharex=True, gridspec_kw={"height_ratios": [1, 1.4]}
    )
    fig.suptitle(f"Estado del sistema — {n_label}", fontsize=13)

    
    ax_estado.plot(time, df["ocupacion_buffer"], color="tab:blue", linewidth=1,
                    label="ocupación búfer")
    
    
    if "ocupacion_ventana" in df.columns:
        ax_estado.plot(time, df["ocupacion_ventana"], color="tab:purple", linewidth=1,
                        label="tamaño ventana de tasa")
        

    ax_estado.set_ylabel("Ocupación (paquetes)")
    ax_estado.legend(loc="upper right", fontsize=8)
    ax_estado.grid(True, alpha=0.3)

    
    ax_eventos.stackplot(
        time, aceptados_bin, tasa_bin, lleno_bin,
        labels=["aceptados", "rechazo por tasa", "rechazo por búfer lleno"],
        colors=["tab:green", "tab:orange", "tab:red"],
    )
    ax_eventos.set_xlabel("Tiempo simulado (ms)")
    ax_eventos.set_ylabel("Paquetes por intervalo de muestreo")
    ax_eventos.legend(loc="upper right", fontsize=8)
    ax_eventos.grid(True, alpha=0.3)

    plt.tight_layout()
    fig.savefig(f"{n_label}_estado_eventos.png", dpi=150)

    
    if es_acumulado:
        fig2, ax2 = plt.subplots(figsize=(9, 4))
        ax2.plot(time, df["aceptados"], label="aceptados", color="tab:green")
        ax2.plot(time, df["rechazados_tasa"], label="rechazados por tasa", color="tab:orange")
        ax2.plot(time, df["rechazados_lleno"], label="rechazados por búfer lleno", color="tab:red")
        ax2.set_yscale("log")
        ax2.set_xlabel("Tiempo simulado (ms)")
        ax2.set_ylabel("Total acumulado (escala log)")
        ax2.set_title(f"Totales acumulados — {n_label}")
        ax2.legend(fontsize=8)
        ax2.grid(True, which="both", alpha=0.3)
        fig2.tight_layout()
        fig2.savefig(f"{n_label}_acumulado_log.png", dpi=150)

    plt.close("all")

print("Listo: se guardó un PNG de estado+eventos (y otro de acumulados si aplica) por cada archivo.")