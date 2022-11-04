#pragma once

enum class BlockType : unsigned int {
    Air = 0,
    Grass,
    Dirt,
    Stone,
    Bedrock,
    Water,
    Sand,
    Gravel,
    Log,
    Leaf
};

class Block {
   public:
    Block();
    ~Block();

    // Getter
    bool IsActive() const { return m_ID != 0 && m_ID != static_cast<unsigned int>(BlockType::Water); }
    inline BlockType GetBlockType() const { return static_cast<BlockType>(m_ID); }
    bool IsTransparent() const { return m_ID == static_cast<unsigned int>(BlockType::Leaf); }

    // Setters
    void SetBlockType(BlockType blockType);

   private:
    unsigned int m_ID;
};