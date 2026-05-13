#pragma once

class BaseEffect
{
public:

	BaseEffect() {}
	virtual ~BaseEffect() {}

	void Init();
	void Update();
	void Draw();
	void Release();

	bool IsActive() { return m_isActive; }

	void SetTexture(KdTexture* tex) { m_tex = tex; }
	void SetPosition(Math::Vector2& pos) { m_pos = pos; }
	void SetActive(bool active) { m_isActive = active; }
	void SetSize(float size) { m_size = size; }

protected:

	void AnimUpdate();

	KdTexture* m_tex = nullptr;
	bool m_isActive = false;
	Math::Vector2 m_pos = {};
	float m_size = 1.0f;

	int m_animIndex = 0;
	float m_animTimer = 0.0f;
	float m_animSpeed = 0.1f;
	int m_maxFrames = 2;

};