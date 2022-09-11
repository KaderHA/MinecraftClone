#pragma once

enum class BlockType : unsigned int {
    Air = 0,
    Grass,
    Dirt,
    Stone,
    Bedrock,
    Water
};

class Block {
   public:
    Block();
    ~Block();

    // Getter
    bool IsActive() const { return m_ID != 0; }
    inline BlockType GetBlockType() const { return static_cast<BlockType>(m_ID); }

    // Setters
    void SetBlockType(BlockType blockType);

   private:
    unsigned int m_ID;
};