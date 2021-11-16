
uniform vec2 u_cacheMapSize;
uniform sampler2D u_redirectMap;


///////////////////////////////////////////////////////////
vec3 sampleRedirectData(vec2 uv)
{
    return round(texture(u_redirectMap, uv).xyz * 255.0f);
}


///////////////////////////////////////////////////////////
vec4 sample(sampler2D tex, vec2 uv, vec3 redirect)
{
    // Redirect data
    vec2 cachePos = redirect.xy;
    float scaleExp = redirect.z;

    // Calculate tile resolution
    vec2 cacheTexSize = textureSize(tex, 0);
    float tileRes = cacheTexSize.x / u_cacheMapSize.x;

    // Calculate tile coordinates
    float tileScale = pow(2.0f, scaleExp);
    vec2 tileCoords = fract(uv * vec2(textureSize(u_redirectMap, 0)) / tileScale);
    tileCoords = (tileCoords * (tileRes - 2.0f) + 1.0f) / tileRes;

    // Calculate cache coordinates
    vec2 cacheCoords = tileRes * (cachePos + tileCoords) / cacheTexSize;

    // Sample cache texture
    return texture(tex, cacheCoords);
}


///////////////////////////////////////////////////////////
vec4 sample(sampler2D tex, vec2 uv)
{
    return sample(tex, uv, sampleRedirectData(uv));
}


///////////////////////////////////////////////////////////
vec4 sampleBase(sampler2D tex, vec2 uv)
{
    // Base texture should always be at (0, 0) tile
    vec2 texSize = textureSize(tex, 0);
    float tileRes = texSize.x / u_cacheMapSize.x;
    vec2 cacheCoords = (uv * (tileRes - 2.0f) + 1.0f) / texSize;

    // Sample cache texture
    return texture(tex, cacheCoords);
}