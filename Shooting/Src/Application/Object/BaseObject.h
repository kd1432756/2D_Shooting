#pragma	once

class BaseObject
{
public:

	void Init();
	void Update();
	void Draw();
	void Release();

private:

	KdTexture m_tex;
	bool m_isActive;
	Math::Vector2 m_pos;
};