
class TextureCommon
{
public:
	TextureCommon();
	virtual ~TextureCommon();
	void SetRepeating(bool repeat);
	void SetAnistropicFiltering(bool enable);
protected:
	bool repeating; // determines whether textures is repeated or clamped
	bool anisotropic; // enables anisotropic filtering for the texture
private:
	TextureCommon(const TextureCommon& in);
	TextureCommon& operator=(const TextureCommon& in);
};

