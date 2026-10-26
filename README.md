# ✦ Graphical Algorithm Visualizer

An interactive Computer Graphics Educational Platform developed in **C (WinBGIm)** and **HTML5 Canvas / JavaScript**. This visualizer provides real-time, step-by-step mathematical animations of fundamental rasterization and clipping algorithms.

---

## 🚀 Live Web Demo
Deploy and access the interactive web version live in your browser:
- **Web Visualizer**: `https://<YOUR-USERNAME>.github.io/<REPO-NAME>/`
- **Technical Report & PDF**: Open `project_report.html` and click **"Save as PDF / Print"**

---

## 🧮 Implemented Algorithms

| Algorithm | Category | Arithmetic Type | Core Feature |
| :--- | :--- | :--- | :--- |
| **DDA Line** | Line Drawing | Floating-Point | Differential increments, `round()` per pixel |
| **Bresenham Line** | Line Drawing | Pure Integer | Error decision parameter ($p = 2\Delta y - \Delta x$) |
| **Midpoint Line** | Line Drawing | Pure Integer | Evaluates geometric midpoint across all 6 slope conditions |
| **Midpoint Circle** | Circle Drawing | Pure Integer | 8-Way symmetry, decision parameter ($d_0 = 1 - R$) |
| **Cohen-Sutherland** | Line Clipping | Bitwise & Float | 4-Bit region outcodes with 9-region trivial accept/reject |
| **Sutherland-Hodgman**| Polygon Clipping | Float Intersect | 4-Stage boundary pipeline satisfying all 4 cases (`IN/OUT`) |

---

## 👥 Project Team & Credits

- **Department:** MCA Department
- **Project Guide:** Dr. Ravikumar Roogi
- **Academic Year:** 2025 – 2027
- **Course:** Computer Graphics & Visualization

### Team Members (Students):
1. **Chaitra N** — UUCMS: `PO2DR25S126006`
2. **Vasanta M** — UUCMS: `PO2DR25S126038`
3. **Ananya S** — UUCMS: `PO2DR25S126002`
4. **Riya N** — UUCMS: `PO2DR25S126023`
5. **Misba M** — UUCMS: `P02DR25S126015`
6. **Iranna K** — UUCMS: `P02DR25S126010`

---

## 💻 How to Run Locally

### 1. Web Application (Zero Installation Needed)
Simply open `index.html` in any web browser (Chrome, Edge, Firefox).

### 2. Windows Native C Application
Using MinGW `g++`:
```cmd
.\build.bat
```
*(or run manually)*:
```cmd
g++ main.c -o visualizer.exe -I. -L. -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 -fpermissive
.\visualizer.exe
```

---

## 🌐 Deploy to GitHub Pages

1. Push this repository to GitHub.
2. Go to **Settings** $\to$ **Pages**.
3. Under **Build and deployment**, select **Source:** `Deploy from a branch`, **Branch:** `main`, folder `/ (root)`.
4. Click **Save** — your site will be live at `https://<YOUR-USERNAME>.github.io/<REPO-NAME>/`!
