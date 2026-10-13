#include "settings/LuigiPixels.h"

#include <math.h>
#include <string.h>

#include "al/Library/LiveActor/ActorActionFunction.h"
#include "al/Library/LiveActor/ActorMovementFunction.h"
#include "al/Library/LiveActor/ActorPoseUtil.h"
#include "al/Library/Screen/ScreenFunction.h"
#include "game/System/GameDataFunction.h"
#include "game/System/GameDataHolderAccessor.h"
#include "game/System/GameDataHolderWriter.h"
#include "helpers/InputHelper.h"
#include "imgui.h"
#include "menu/Menu.h"
#include "src/settings/SettingsMgr.h"

namespace btt {

struct LuigiPixelSpot {
    const char* kingdomName;
    float x;
    float y;
    float z;
    float radius;
};

// Authentic in-game coordinates captured directly via Info window (Trans)
// Calibrated with reduced radius (250.0f) for strict on-spot ground pounds
static constexpr LuigiPixelSpot sSpots[] = {
    // 0. Royaume Champignon (Mushroom Kingdom)
    { "Royaume Champignon", -6184.500f, 2600.157f, -12782.371f, 250.0f },

    // 1. Spot 1
    { "Spot #1", -14487.712f, -1021.293f, 4745.253f, 250.0f },

    // 2. Pays des Chapeaux (Cap Kingdom)
    { "Pays des Chapeaux", 8274.511f, 550.000f, -7679.649f, 250.0f },

    // 3. Spot 3
    { "Spot #3", 369.770f, 3591.533f, -7286.451f, 250.0f },

    // 4. Spot 4
    { "Spot #4", 21211.375f, -1491.196f, 7220.243f, 250.0f },

    // 5. Pays des Sables #2 (Sand Kingdom - Tostarena)
    { "Pays des Sables #2", 20128.512f, -935.902f, -15474.126f, 250.0f },

    // 6. Pays du Lac (Lake Kingdom)
    { "Pays du Lac", -17670.291f, -2550.000f, -5559.074f, 250.0f },

    // 7. Pays des Chutes (Cascade Kingdom)
    { "Pays des Chutes", 1881.059f, 4400.000f, 156.173f, 250.0f },

    // 8. Pays de la Mer (Seaside Kingdom)
    { "Pays de la Mer", 3390.384f, -2.028f, -4925.248f, 250.0f },

    // 9. Spot 9
    { "Spot #9", 3192.615f, 400.000f, 7391.288f, 250.0f },

    // 10. Pays Perdu (Lost Kingdom)
    { "Pays Perdu", 1070.010f, 2199.789f, -1999.873f, 250.0f },

    // 11. Spot 11
    { "Spot #11", -4550.287f, 11025.000f, -13794.618f, 250.0f },

    // 12. New Donk City (Metro Kingdom - Panneaux de rue)
    { "New Donk City", -1443.957f, 0.000f, -1367.683f, 250.0f },

    // 13. Pays des Neiges (Snow Kingdom)
    { "Pays des Neiges", 2320.671f, 1250.000f, 2814.773f, 250.0f },

    // 14. Spot 14
    { "Spot #14", 3909.112f, -150.000f, -4510.523f, 250.0f },

    // 15. Spot 15
    { "Spot #15", -19081.914f, 3249.763f, -21191.645f, 250.0f },

    // 16. Pays de Bowser (Bowser's Kingdom)
    { "Pays de Bowser", 48877.582f, 53077.910f, -73224.305f, 250.0f },

    // 17. Spot 17
    { "Spot #17", 7605.395f, 1449.869f, 22411.375f, 250.0f }
};

static constexpr size_t sSpotCount = sizeof(sSpots) / sizeof(sSpots[0]);

// Tracks discovery per spot: first find gives 200 coins, subsequent finds give 10 coins
static bool sDiscoveredSpots[sSpotCount] = { false };
static int sPendingCoins = 0;
static int sCooldown = 0;

// Official 16x16 8-bit NES Luigi sprite matrix - Standing Pose
// 0: Transparent, 1: Green, 2: Blue, 3: Skin, 4: Hair, 5: Shoes, 6: Yellow, 7: White
static const uint8_t sLuigiSprite[16][16] = {
    { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 4, 4, 4, 3, 3, 4, 3, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 4, 3, 4, 3, 3, 3, 4, 3, 3, 3, 0, 0, 0 },
    { 0, 0, 0, 4, 3, 4, 4, 3, 3, 3, 4, 3, 3, 3, 0, 0 },
    { 0, 0, 0, 4, 4, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 7, 7, 1, 2, 6, 2, 2, 6, 2, 1, 7, 7, 0, 0 },
    { 0, 0, 7, 7, 7, 2, 2, 2, 2, 2, 2, 7, 7, 7, 0, 0 },
    { 0, 0, 7, 7, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 0, 0 },
    { 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0 },
    { 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0 },
    { 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0 }
};

// Official 16x16 8-bit NES Luigi sprite matrix - Jump / Running Pose (as in video 1011.mp4)
static const uint8_t sLuigiJumpSprite[16][16] = {
    { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 4, 4, 4, 3, 3, 4, 3, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 4, 3, 4, 3, 3, 3, 4, 3, 3, 3, 0, 0, 0 },
    { 0, 0, 0, 4, 3, 4, 4, 3, 3, 3, 4, 3, 3, 3, 0, 0 },
    { 0, 0, 0, 4, 4, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0 },
    { 0, 0, 7, 7, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 7, 7, 7, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 7, 7, 7, 1, 1, 1, 2, 2, 2, 2, 7, 7, 0, 0, 0 },
    { 0, 0, 7, 7, 1, 1, 2, 6, 2, 2, 7, 7, 7, 0, 0, 0 },
    { 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 7, 7, 0, 0, 0 },
    { 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0 },
    { 0, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0 },
    { 5, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0 }
};

struct ActivePixelLuigiEntity {
    bool active = false;
    sead::Vector3f pos;
    int timer = 0;              // frames active (max 600 frames = 10 sec)
    float twirl = 0.0f;         // horizontal spin angle
    float height = 0.0f;        // height above ground
    int cappySpinFrames = 0;    // twirl frames when hit by Cappy / Mario
    al::LiveActor* playerRef = nullptr;
};

static ActivePixelLuigiEntity sActiveLuigi;

void LuigiPixels::update(al::LiveActor* player) {
    if (!player) return;

    auto* s = SettingsMgr::instance()->getSettings();
    if (!s || !s->mIsEnableLuigiPixels) return;

    // Detect player actor recreate / scene restart
    if (sActiveLuigi.playerRef != nullptr && sActiveLuigi.playerRef != player) {
        resetDiscoveredSpots();
    }
    sActiveLuigi.playerRef = player;

    if (sCooldown > 0) sCooldown--;

    // Progressive coin payout: stream coins frame-by-frame with authentic audio/HUD counter roll
    if (sPendingCoins > 0) {
        int step = (sPendingCoins > 50) ? 4 : 2;
        if (sPendingCoins < step) {
            step = sPendingCoins;
        }
        sPendingCoins -= step;
        GameDataFunction::addCoin(GameDataHolderWriter(player), step);
    }

    const sead::Vector3f& playerPos = al::getTrans(player);

    static float sPrevY = 0.0f;
    static int sPlungeFrames = 0;
    static bool sPlungingWithZL = false;

    float dy = sPrevY - playerPos.y; // > 0 when falling downwards
    sPrevY = playerPos.y;

    float vSpeed = al::calcSpeedV(player);

    bool isZL_ZR = InputHelper::isHoldZL() || InputHelper::isHoldZR() ||
                   InputHelper::isPressZL() || InputHelper::isPressZR();

    bool isGroundPoundLanding = false;

    // Detect Ground Pound physics:
    // Mario is airborne and plunging downwards at high speed with ZL/ZR held or pressed
    if (dy > 3.0f || dy < -3.0f || fabsf(vSpeed) > 4.0f) {
        if ((dy > 6.0f || vSpeed < -10.0f) && isZL_ZR) {
            sPlungingWithZL = true;
            sPlungeFrames++;
        }
    } else {
        // Mario hits the ground (vertical downward plunge halted)
        if (sPlungingWithZL && sPlungeFrames >= 2) {
            isGroundPoundLanding = true;
        }
        sPlungingWithZL = false;
        sPlungeFrames = 0;
    }

    if (isGroundPoundLanding && sCooldown == 0) {
        for (size_t i = 0; i < sSpotCount; i++) {
            const auto& spot = sSpots[i];

            float dx = playerPos.x - spot.x;
            float dY = playerPos.y - spot.y;
            float dz = playerPos.z - spot.z;

            float dist2 = dx * dx + dz * dz;
            float r = spot.radius;
            // Strict distance check (250 units) and height check (180 units)
            if (dist2 <= r * r && fabsf(dY) <= 180.0f) {
                bool isFirstTime = !sDiscoveredSpots[i];
                if (isFirstTime) {
                    sDiscoveredSpots[i] = true;
                    sPendingCoins += 200;
                    Menu::instance()->setPopupText("Pixel Luigi [%s]! (+200 coins)", spot.kingdomName);
                } else {
                    // Official base-game behavior: 10 coins on repeat
                    sPendingCoins += 10;
                    Menu::instance()->setPopupText("Pixel Luigi [%s] (+10 coins)", spot.kingdomName);
                }

                sCooldown = 120;

                // Spawn Pixel Luigi exactly where Mario smashed the ground
                sActiveLuigi.active = true;
                sActiveLuigi.pos = sead::Vector3f(playerPos.x, playerPos.y, playerPos.z);
                sActiveLuigi.timer = 0;
                sActiveLuigi.twirl = 0.0f;
                sActiveLuigi.height = 0.0f;
                sActiveLuigi.cappySpinFrames = 0;
                break;
            }
        }
    }

    // Interaction: Mario or Cappy strikes Luigi -> trigger mid-air spin pirouette!
    if (sActiveLuigi.active && sActiveLuigi.timer >= 72) {
        float dMarioX = playerPos.x - sActiveLuigi.pos.x;
        float dMarioY = playerPos.y - sActiveLuigi.pos.y;
        float dMarioZ = playerPos.z - sActiveLuigi.pos.z;
        float dMario2 = dMarioX * dMarioX + dMarioZ * dMarioZ;

        // When Mario or Cappy makes contact (within 190 units)
        if (dMario2 < 190.0f * 190.0f && fabsf(dMarioY) < 180.0f) {
            if (sActiveLuigi.cappySpinFrames == 0) {
                sActiveLuigi.cappySpinFrames = 48; // Jump and 360 twirl!
            }
        }
    }
}

void LuigiPixels::draw() {
    if (!sActiveLuigi.active || !sActiveLuigi.playerRef) return;

    auto* s = SettingsMgr::instance()->getSettings();
    if (!s || !s->mIsEnableLuigiPixels) return;

    sActiveLuigi.timer++;
    const int totalDuration = 600; // ~10 seconds
    if (sActiveLuigi.timer >= totalDuration) {
        sActiveLuigi.active = false;
        return;
    }

    // Faithful reproduction of animation in video 1011.mp4:
    // Phase 1 (0..44): High launch pop-out jump arc (leaps high into the air in SMB1 jump pose, twirling)
    // Phase 2 (45..58): Lands flat on the grass on his belly (prone splat horizontal)
    // Phase 3 (59..72): Cute bounce up to upright standing on his feet
    // Phase 4 (73..600): Stands proud on the ground with white indicator arrow (⬇) and 2 floating rotating golden coins!
    // Phase 5 (Cappy hit): Leaps into the air and does a rapid 360° pirouette twirl with sparkles!

    bool isJumping = false;
    bool isProne = false;
    const uint8_t (*activeSprite)[16] = sLuigiSprite;

    if (sActiveLuigi.timer < 45) {
        // Phase 1: High launch pop-out jump arc
        float t = (float)sActiveLuigi.timer / 45.0f;
        sActiveLuigi.height = sinf(t * 3.14159265f) * 280.0f;
        sActiveLuigi.twirl += 0.26f;
        activeSprite = sLuigiJumpSprite;
        isJumping = true;
    } else if (sActiveLuigi.timer < 59) {
        // Phase 2: Prone splat flat on the ground (as in 1011.mp4 frame 17)
        sActiveLuigi.height = 0.0f;
        sActiveLuigi.twirl = 0.0f;
        activeSprite = sLuigiJumpSprite;
        isProne = true;
    } else if (sActiveLuigi.timer < 73) {
        // Phase 3: Cute bounce to upright standing
        float bT = (float)(sActiveLuigi.timer - 59) / 14.0f;
        sActiveLuigi.height = sinf(bT * 3.14159265f) * 45.0f;
        sActiveLuigi.twirl = 0.0f;
        activeSprite = sLuigiSprite;
    } else if (sActiveLuigi.cappySpinFrames > 0) {
        // Phase 5: Cappy strike reaction -> leaps into air and twirls!
        sActiveLuigi.cappySpinFrames--;
        float p = 1.0f - ((float)sActiveLuigi.cappySpinFrames / 48.0f);
        sActiveLuigi.height = sinf(p * 3.14159265f) * 160.0f;
        sActiveLuigi.twirl += 0.44f;
        activeSprite = sLuigiJumpSprite;
        isJumping = true;
    } else {
        // Phase 4: Idle standing on the ground
        sActiveLuigi.height = 0.0f;
        sActiveLuigi.twirl = 0.0f;
        activeSprite = sLuigiSprite;
    }

    // Alpha fade-out in final 40 frames
    float alpha = 1.0f;
    if (sActiveLuigi.timer > (totalDuration - 40)) {
        alpha = (float)(totalDuration - sActiveLuigi.timer) / 40.0f;
        if (alpha < 0.0f) alpha = 0.0f;
    }

    // Calculate 3D world position
    sead::Vector3f worldPos = sActiveLuigi.pos;
    worldPos.y += sActiveLuigi.height;

    // Project 3D position to 2D screen coordinates
    sead::Vector2f screenPos(0.0f, 0.0f);
    al::calcScreenPosFromWorldPos(&screenPos, sActiveLuigi.playerRef, worldPos);

    // Visibility bounds check
    ImGuiIO& io = ImGui::GetIO();
    float margin = 200.0f;
    if (screenPos.x < -margin || screenPos.x > io.DisplaySize.x + margin ||
        screenPos.y < -margin || screenPos.y > io.DisplaySize.y + margin) {
        return;
    }

    // Calculate distance to camera / player to scale perspective naturally
    const sead::Vector3f& playerPos = al::getTrans(sActiveLuigi.playerRef);
    float dx = playerPos.x - worldPos.x;
    float dy = playerPos.y - worldPos.y;
    float dz = playerPos.z - worldPos.z;
    float dist = sqrtf(dx * dx + dy * dy + dz * dz);

    // Perspective scaling (closer = bigger)
    float basePixelSize = 3500.0f / (dist + 400.0f);
    if (basePixelSize < 1.8f) basePixelSize = 1.8f;
    if (basePixelSize > 9.0f) basePixelSize = 9.0f;

    // Horizontal spin scale
    float spinScale = isJumping ? cosf(sActiveLuigi.twirl) : 1.0f;
    float pixelW = basePixelSize * spinScale;
    float pixelH = basePixelSize;

    // Sprite total dimensions (16x16)
    float totalW = fabsf(pixelW * 16.0f);
    float totalH = pixelH * 16.0f;
    float originX = screenPos.x - (pixelW * 8.0f);
    float originY = screenPos.y - (isProne ? (pixelH * 4.0f) : totalH);

    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    if (!drawList) return;

    // Ground impact dust shockwave (first 25 frames, exactly as in 1011.mp4)
    if (sActiveLuigi.timer < 25) {
        float dustProg = (float)sActiveLuigi.timer / 25.0f;
        float dustRadius = 25.0f + dustProg * 85.0f;
        for (int pIdx = 0; pIdx < 8; pIdx++) {
            float pAngle = pIdx * (3.14159265f / 4.0f);
            sead::Vector3f puffWorld = sActiveLuigi.pos;
            puffWorld.x += cosf(pAngle) * dustRadius;
            puffWorld.z += sinf(pAngle) * dustRadius;
            puffWorld.y += 4.0f + sinf(dustProg * 3.14159265f) * 12.0f;

            sead::Vector2f puffScreen(0.0f, 0.0f);
            al::calcScreenPosFromWorldPos(&puffScreen, sActiveLuigi.playerRef, puffWorld);

            float pSize = basePixelSize * (1.8f + dustProg * 2.2f);
            uint32_t puffCol = IM_COL32(245, 245, 255, (int)((1.0f - dustProg) * 175 * alpha));
            drawList->AddCircleFilled(ImVec2(puffScreen.x, puffScreen.y), pSize, puffCol, 12);
        }
    }

    // Ground shadow projection (oval beneath Luigi, scaling with altitude)
    sead::Vector2f shadowScreen(0.0f, 0.0f);
    al::calcScreenPosFromWorldPos(&shadowScreen, sActiveLuigi.playerRef, sActiveLuigi.pos);
    float shadowScale = 1.0f - (sActiveLuigi.height / 350.0f) * 0.45f;
    if (shadowScale < 0.4f) shadowScale = 0.4f;
    float shadowRadiusX = fabsf(basePixelSize * 7.0f * shadowScale);
    float shadowRadiusY = fabsf(basePixelSize * 2.8f * shadowScale);
    uint32_t shadowCol = IM_COL32(10, 10, 15, (int)(110 * alpha * shadowScale));
    drawList->AddEllipseFilled(ImVec2(shadowScreen.x, shadowScreen.y), ImVec2(shadowRadiusX, shadowRadiusY), shadowCol, 0.0f, 16);

    // Sprite Colors with current alpha
    uint8_t a = (uint8_t)(255 * alpha);
    uint32_t colGreen   = IM_COL32(30, 190, 45, a);    // Cap & shirt
    uint32_t colBlue    = IM_COL32(35, 95, 230, a);    // Overalls
    uint32_t colSkin    = IM_COL32(255, 205, 150, a);  // Skin tone
    uint32_t colHair    = IM_COL32(100, 50, 15, a);    // Mustache / hair
    uint32_t colShoe    = IM_COL32(85, 40, 10, a);     // Shoes
    uint32_t colYellow  = IM_COL32(255, 225, 30, a);   // Buttons
    uint32_t colWhite   = IM_COL32(245, 245, 245, a);  // White Gloves / Arrow

    // Render the 16x16 8-bit sprite blocks
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            // When prone (lying flat on ground), rotate 90° so head is on right and shoes on left
            int srcR = isProne ? (15 - col) : row;
            int srcC = isProne ? row : col;
            uint8_t val = activeSprite[srcR][srcC];
            if (val == 0) continue;

            uint32_t color = 0;
            switch (val) {
                case 1: color = colGreen; break;
                case 2: color = colBlue; break;
                case 3: color = colSkin; break;
                case 4: color = colHair; break;
                case 5: color = colShoe; break;
                case 6: color = colYellow; break;
                case 7: color = colWhite; break;
            }

            float px0 = originX + (col * pixelW);
            float py0 = originY + (row * pixelH);
            float px1 = px0 + pixelW;
            float py1 = py0 + pixelH;

            // Ensure min/max coordinates are in correct order even during spin flip
            float minX = px0 < px1 ? px0 : px1;
            float maxX = px0 < px1 ? px1 : px0;
            float minY = py0 < py1 ? py0 : py1;
            float maxY = py0 < py1 ? py1 : py0;

            drawList->AddRectFilled(ImVec2(minX, minY), ImVec2(maxX, maxY), color);
        }
    }

    // White indicator down-arrow (⬇) hovering above Luigi's head (as in video 1011.mp4)
    if (sActiveLuigi.timer >= 65) {
        float arrowBob = sinf((float)sActiveLuigi.timer * 0.14f) * 4.0f;
        sead::Vector3f arrowWorld = sActiveLuigi.pos;
        arrowWorld.y += sActiveLuigi.height + 72.0f + arrowBob;

        sead::Vector2f arrowScreen(0.0f, 0.0f);
        al::calcScreenPosFromWorldPos(&arrowScreen, sActiveLuigi.playerRef, arrowWorld);

        float arrowScale = basePixelSize * 1.5f;
        if (arrowScale < 4.0f) arrowScale = 4.0f;
        if (arrowScale > 14.0f) arrowScale = 14.0f;

        float stemW = arrowScale * 1.4f;
        float stemH = arrowScale * 2.2f;
        float tipW  = arrowScale * 3.4f;
        float tipH  = arrowScale * 1.8f;

        // Shadow / dark border
        drawList->AddRectFilled(
            ImVec2(arrowScreen.x - stemW * 0.5f - 1.5f, arrowScreen.y - stemH - tipH - 1.5f),
            ImVec2(arrowScreen.x + stemW * 0.5f + 1.5f, arrowScreen.y - tipH + 1.5f),
            IM_COL32(0, 0, 0, (int)(160 * alpha))
        );
        drawList->AddTriangleFilled(
            ImVec2(arrowScreen.x - tipW * 0.5f - 2.0f, arrowScreen.y - tipH - 1.0f),
            ImVec2(arrowScreen.x + tipW * 0.5f + 2.0f, arrowScreen.y - tipH - 1.0f),
            ImVec2(arrowScreen.x, arrowScreen.y + 2.0f),
            IM_COL32(0, 0, 0, (int)(160 * alpha))
        );

        // White arrow
        drawList->AddRectFilled(
            ImVec2(arrowScreen.x - stemW * 0.5f, arrowScreen.y - stemH - tipH),
            ImVec2(arrowScreen.x + stemW * 0.5f, arrowScreen.y - tipH),
            colWhite
        );
        drawList->AddTriangleFilled(
            ImVec2(arrowScreen.x - tipW * 0.5f, arrowScreen.y - tipH),
            ImVec2(arrowScreen.x + tipW * 0.5f, arrowScreen.y - tipH),
            ImVec2(arrowScreen.x, arrowScreen.y),
            colWhite
        );
    }

    // Two giant 3D rotating golden coins floating above the arrow (as in video 1011.mp4)
    if (sActiveLuigi.timer >= 65) {
        float coinBob = sinf((float)sActiveLuigi.timer * 0.08f) * 3.0f;
        float coinWorldHeights[2] = { 108.0f + coinBob, 150.0f + coinBob };

        for (int cIdx = 0; cIdx < 2; cIdx++) {
            sead::Vector3f coinWorld = sActiveLuigi.pos;
            coinWorld.y += sActiveLuigi.height + coinWorldHeights[cIdx];

            sead::Vector2f coinScreen(0.0f, 0.0f);
            al::calcScreenPosFromWorldPos(&coinScreen, sActiveLuigi.playerRef, coinWorld);

            float coinR = basePixelSize * 3.4f;
            if (coinR < 6.0f) coinR = 6.0f;
            if (coinR > 26.0f) coinR = 26.0f;

            // 3D rotation around vertical axis
            float coinAngle = (float)sActiveLuigi.timer * 0.07f + (cIdx * 0.5f);
            float cosAngle = cosf(coinAngle);
            float coinRx = fabsf(coinR * cosAngle);
            if (coinRx < 1.5f) coinRx = 1.5f;

            // Outer golden ring
            drawList->AddEllipseFilled(
                ImVec2(coinScreen.x, coinScreen.y),
                ImVec2(coinRx, coinR),
                IM_COL32(235, 175, 10, a),
                0.0f, 24
            );
            // Inner yellow shine
            drawList->AddEllipseFilled(
                ImVec2(coinScreen.x, coinScreen.y),
                ImVec2(coinRx * 0.78f, coinR * 0.78f),
                IM_COL32(255, 235, 75, a),
                0.0f, 24
            );
            // Inner embossed bar (visible when face is towards camera)
            if (coinRx > coinR * 0.45f) {
                float barW = coinRx * 0.22f;
                float barH = coinR * 0.52f;
                drawList->AddRectFilled(
                    ImVec2(coinScreen.x - barW, coinScreen.y - barH),
                    ImVec2(coinScreen.x + barW, coinScreen.y + barH),
                    IM_COL32(215, 145, 10, a)
                );
            }
        }
    }

    // Sparkle / Retro 8-bit star particle effect during pop out & active spin
    int sparkleCount = (sActiveLuigi.timer < 50 || sActiveLuigi.cappySpinFrames > 0) ? 6 : 4;
    for (int sIdx = 0; sIdx < sparkleCount; sIdx++) {
        float sparkAngle = (float)sActiveLuigi.timer * 0.12f + (sIdx * (6.2831853f / (float)sparkleCount));
        float sparkDist = (totalW * 0.75f) + sinf((float)sActiveLuigi.timer * 0.2f + sIdx) * 8.0f;
        float sx = screenPos.x + cosf(sparkAngle) * sparkDist;
        float sy = (screenPos.y - totalH * 0.5f) + sinf(sparkAngle) * (sparkDist * 0.5f);
        float sparkSize = basePixelSize * 1.1f;

        uint32_t sparkCol = (sIdx % 2 == 0) ? IM_COL32(255, 240, 90, (int)(220 * alpha))
                                            : IM_COL32(110, 255, 130, (int)(220 * alpha));
        drawList->AddRectFilled(ImVec2(sx - sparkSize, sy - sparkSize),
                                ImVec2(sx + sparkSize, sy + sparkSize), sparkCol);
    }
}

void LuigiPixels::resetDiscoveredSpots() {
    for (size_t i = 0; i < sSpotCount; i++) {
        sDiscoveredSpots[i] = false;
    }
    sPendingCoins = 0;
    sCooldown = 0;
    sActiveLuigi.active = false;
    sActiveLuigi.timer = 0;
    sActiveLuigi.cappySpinFrames = 0;
}

} // namespace btt
