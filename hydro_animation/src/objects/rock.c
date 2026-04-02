#include "rock.h"
#include "../wrapper/draw_ellipse.h"
#include "../wrapper/draw_line.h"
#include "coords.h"

void DrawRock(int cx, int cy, int rx, int ry) {
    Color baseCol = (Color){110, 110, 110, 255};
    Color highCol = (Color){155, 155, 155, 255};
    Color shadCol = (Color){75,  75,  75,  255};

    // 1. Siluet bentukan batu tak simetris (menggabungkan beberapa gumpalan elips)
    Wrapper_DrawEllipseFilled(cx, cy, rx, ry, baseCol);
    Wrapper_DrawEllipseFilled(cx - (int)(rx*0.6f), cy - (int)(ry*0.2f), (int)(rx*0.5f), (int)(ry*0.7f), baseCol);
    Wrapper_DrawEllipseFilled(cx + (int)(rx*0.5f), cy + (int)(ry*0.1f), (int)(rx*0.6f), (int)(ry*0.6f), baseCol);

    // 2. Shading bagian bawah/kasar menumpu ke tanah (nilai Y lebih rendah)
    Wrapper_DrawEllipseFilled(cx, cy - (int)(ry*0.4f), (int)(rx*0.8f), (int)(ry*0.5f), shadCol);
    Wrapper_DrawEllipseFilled(cx + (int)(rx*0.4f), cy - (int)(ry*0.3f), (int)(rx*0.5f), (int)(ry*0.4f), shadCol);

    // 3. Highlight pantulan cahaya matahari natural di sisi puncak/atas (nilai Y lebih tinggi)
    Wrapper_DrawEllipseFilled(cx - (int)(rx*0.2f), cy + (int)(ry*0.4f), (int)(rx*0.6f), (int)(ry*0.4f), highCol);
    Wrapper_DrawEllipseFilled(cx - (int)(rx*0.5f), cy + (int)(ry*0.2f), (int)(rx*0.3f), (int)(ry*0.3f), highCol);

    // 4. Guratan/retakan batuan keras agar makin nyata (fracture lines)
    Wrapper_DrawLineThick(cx - (int)(rx*0.4f), cy + (int)(ry*0.1f), cx + (int)(rx*0.2f), cy - (int)(ry*0.2f), 2, shadCol);
    Wrapper_DrawLineThick(cx + (int)(rx*0.2f), cy - (int)(ry*0.2f), cx + (int)(rx*0.5f), cy + (int)(ry*0.3f), 2, shadCol);
    Wrapper_DrawLineThick(cx - (int)(rx*0.1f), cy + (int)(ry*0.3f), cx + (int)(rx*0.1f), cy, 2, shadCol);
}
