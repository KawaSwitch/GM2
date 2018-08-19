#pragma once

// 描画アシスト
template <class Anything>
void DrawWithoutSetting(const Anything &obj)
{
    // デバグ描画用
    obj.Draw();
}
