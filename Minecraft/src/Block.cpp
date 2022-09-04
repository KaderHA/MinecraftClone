#include "Block.hpp"

Block::Block() : m_ID(0) {
}

Block::~Block() {
}

void Block::SetBlockType(BlockType blockType) {
    m_ID = static_cast<unsigned int>(blockType);
}
