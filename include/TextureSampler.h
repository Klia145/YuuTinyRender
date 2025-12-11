#include"tgaimage.h"
#include"vec2.h"
enum class SamplingMode{
    NEAREST,
    BILINEAR,
    TRILINEAR
};
class TextureSampler{
    private:
    const TGAImage* texture;
    SamplingMode mode;

    TGAColor sample_nearest(vec2 uv)const;
    TGAColor sample_bilinear(vec2 uv) const;
    
    public:
    TextureSampler(const TGAImage* tex,SamplingMode sampling_mode=SamplingMode::NEAREST);


    void setMode(SamplingMode sampling_mode);

    SamplingMode getMode()const;

    void setTexture(const TGAImage* tex);

    TGAColor sample(vec2 uv) const;

    TGAColor sample(float u,float v)const;
    
};