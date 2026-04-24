#pragma	once

class BaseObject
{
public:
	BaseObject() {}
	virtual ~BaseObject() {}

	void Init();
	void Update();
	void Draw();
	void Release();

	void SetPosition(Math::Vector2& pos) { m_pos = pos; }
	void SetTexture(KdTexture* tex) { m_tex = tex; }
	void SetActive(bool active) { m_isActive = active; }

	bool IsActive() { return m_isActive; }

protected:

	KdTexture* m_tex;
	bool m_isActive = false;
	Math::Vector2 m_pos;
};