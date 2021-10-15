
uniform float u_tileSize;
uniform usampler2D u_indexMap;


///////////////////////////////////////////////////////////
vec4 sample(sampler2D tex, vec2 uv)
{
    // Sample index map
    vec2 cachePos = texture(u_indexMap, uv).xy;

    // Calculate tile coordinates
    vec2 tileCoords = fract(uv * vec2(textureSize(u_indexMap)));
    tileCoords = (tileCoords * (u_tileSize - 1.0f) + 0.5f) / u_tileSize;

    // Calculate cache coordinates
    vec2 cacheCoords = u_tileSize * (cachePos + tileCoords) / textureSize(tex);

    // Sample cache texture
    return texture(tex, cacheCoords);
}


///////////////////////////////////////////////////////////
vec4 sampleBase(sampler2D tex, vec2 uv)
{    
    // Base texture should always be at (0, 0) tile
    vec2 cacheCoords = (uv * (u_tileSize - 1.0f) + 0.5f) / textureSize(tex);

    // Sample cache texture
    return texture(tex, tileCoords);
}