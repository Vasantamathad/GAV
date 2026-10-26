# 🚀 How to Deploy Your Graphics Algorithm Visualizer Web App

This guide walks you through deploying your **Graphical Algorithm Visualizer** online so that anyone (professors, evaluators, classmates) can open and interact with it on their phone, laptop, or tablet via a public web link.

The project is fully client-side (HTML5 Canvas + CSS + JavaScript) with zero build steps and zero dependencies.

---

## ⚡ Method 1: Netlify Drop (Fastest — 15 Seconds, No Terminal Needed)

This is the easiest method if you want an instant live URL without using Git or commands.

1. Open your browser and go to: **[https://app.netlify.com/drop](https://app.netlify.com/drop)**
2. Sign up or log in (free with GitHub or Email).
3. Open Windows File Explorer and navigate to:
   `c:\Users\VASANTA M\Downloads\Graphical-Algo-Visulizer-main`
4. **Drag and drop** the folder directly into the Netlify Drop area on your screen.
5. In 5–10 seconds, Netlify will deploy your site and give you a free public link (e.g., `https://graphics-algo-visualizer.netlify.app`).

---

## 🌐 Method 2: GitHub Pages (Recommended for College & Resumes)

This hosts your project directly under your GitHub account for free forever.

### Step 1: Initialize Git and Push to GitHub
Open your terminal (PowerShell or Git Bash) in your project directory:

```bash
cd "c:\Users\VASANTA M\Downloads\Graphical-Algo-Visulizer-main"
git init
git add .
git commit -m "Initial release of Graphics Algorithm Visualizer Web App"
```

Create a new public repository on [GitHub](https://github.com/new) named `Graphics-Algorithm-Visualizer`, then run:

```bash
git branch -M main
git remote add origin https://github.com/<YOUR-GITHUB-USERNAME>/Graphics-Algorithm-Visualizer.git
git push -u origin main
```

### Step 2: Enable GitHub Pages
1. Go to your repository on GitHub.
2. Click **Settings** (top menu bar) -> Select **Pages** (in the left sidebar).
3. Under **Build and deployment**:
   - **Source**: `Deploy from a branch`
   - **Branch**: Select `main` and folder `/ (root)`
4. Click **Save**.
5. Within 1–2 minutes, your project is live at:
   **`https://<YOUR-GITHUB-USERNAME>.github.io/Graphics-Algorithm-Visualizer/`**

---

## 🔺 Method 3: Vercel (1-Command Terminal Deploy)

If you have Node.js installed on your computer:

```bash
cd "c:\Users\VASANTA M\Downloads\Graphical-Algo-Visulizer-main"
npx -y vercel
```

- When prompted:
  - `Set up and deploy?` Press **Y**
  - `Which scope?` Press **Enter**
  - `Link to existing project?` Press **N**
  - `Project name?` Press **Enter** (or type a name)
  - `In which directory is your code located?` Press **Enter** (for `./`)
- Vercel will output your instant production URL (e.g., `https://graphics-visualizer.vercel.app`).

---

## 📦 What Is Included in the Deployed Web App:

1. **Interactive Real-Time Canvas**:
   - DDA Line Drawing
   - Bresenham Line Drawing
   - Midpoint Line Drawing (All 6 slope conditions)
   - Midpoint Circle Drawing (8-way symmetry)
   - Cohen-Sutherland Line Clipping (9-region outcodes)
   - Sutherland-Hodgman Polygon Clipping (All 4 conditions: `IN->IN`, `IN->OUT`, `OUT->OUT`, `OUT->IN`)
2. **Side Telemetry HUD**: Real-time decision parameters ($d, p$), coordinates, and condition branches.
3. **Control Bar**: Play, Pause, Single-Step, Reset, Speed Slider.
4. **Comparison Table Modal**: High-contrast, formatted comparison of all algorithms.
5. **About Project Modal**: MCA Department, Guide: Dr. Ravikumar Roogi, Academic Year 2025–2027, and all 6 team members with UUCMS registration numbers.
6. **Project Report (PDF)**: Clickable link opening `project_report.html` ready to print or save to PDF.
