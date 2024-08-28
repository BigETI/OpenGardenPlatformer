#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <iostream>

#include <OGP/Cells/EGardenCellType.hpp>
#include <OGP/Collections/EGridResizingRule.hpp>
#include <OGP/Entities/EGardenEntityType.hpp>
#include <OGP/Entities/GardenEntityData.hpp>
#include <OGP/Environment/GardenCollectionData.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenAirCellTypeVariant.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenCellType.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenClimbableCellTypeVariant.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenDoorCellTypeVariant.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenEntityType.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenFlowerEntityVariantType.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenFruitEntityVariantType.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenGroundCellTypeVariant.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenKeyEntityVariantType.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenLiftEntityVariantType.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenVegetableEntityVariantType.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenWaterCellTypeVariant.hpp>
#include <OGP/Environment/Legacy/ELegacyGardenWormEntityVariantType.hpp>
#include <OGP/Math/Vector2.hpp>
#include <OGP/Serialization/LegacyGardenCollectionDeserializer.hpp>

using namespace OGP::Cells;
using namespace OGP::Collections;
using namespace OGP::Entities;
using namespace OGP::Environment;
using namespace OGP::Environment::Legacy;
using namespace OGP::Math;
using namespace OGP::Serialization;
using namespace std;

constexpr const array<uint8_t, 5U> legacyGardenCollectionStreamHeader({ 0x01, 0x67, 0x07, 0x04, 0x14 });
static const string legacyGardenCollectionStreamHeaderString("FB: Daisy's Garden 2");
constexpr const array<uint8_t, 3U> legacyGardenCollectionStreamHeaderUnknownBytes({ 0x07, 0x00, 0x00 });
constexpr const array<uint8_t, 5U> legacyGardenCollectionRemainingUnknownInternalConstant({ 0xFF, 0x01, 0x00, 0x09, 0x00 });
static const string legacyGardenCollectionInternalHeaderString("CDaisygPg");
constexpr const uint32_t legacyGardenCollectionGardenSeparator(0x00028001);

LegacyGardenCollectionDeserializer::LegacyGardenCollectionDeserializer() {
	// ...
}

bool LegacyGardenCollectionDeserializer::TryDeserializingStream(istream& inputStream, GardenCollectionData& result) const {
	if (!inputStream.good()) {
		return false;
	}
	{
		array<uint8_t, legacyGardenCollectionStreamHeader.size()> header;
		if (!TryReadingArray<uint8_t, header.size()>(inputStream, header) || (header != legacyGardenCollectionStreamHeader)) {
			cerr << "Garden file header does not match expected garden file header." << endl;
			return false;
		}
	}
	{
		string header_string;
		if (!TryReadingRawString(inputStream, legacyGardenCollectionStreamHeaderString.length(), header_string) || (header_string != legacyGardenCollectionStreamHeaderString)) {
			cerr << "Garden file header string does not match expected garden file header string." << endl;
			return false;
		}
	}
	{
		array<uint8_t, legacyGardenCollectionStreamHeaderUnknownBytes.size()> header_unknown_bytes;
		if (!TryReadingArray<uint8_t, header_unknown_bytes.size()>(inputStream, header_unknown_bytes) || (header_unknown_bytes != legacyGardenCollectionStreamHeaderUnknownBytes)) {
			cerr << "Garden file header unknown bytes does not match expected garden file header unknown bytes." << endl;
			return false;
		}
	}
	if (!TryReadingAndDecryptingDGFString(inputStream, result.editPassword)) {
		cerr << "Failed to read edit password." << endl;
		return false;
	}
	if (!TryReadingAndDecryptingDGFString(inputStream, result.playPassword)) {
		cerr << "Failed to read play password." << endl;
		return false;
	}
	{
		string apply_play_password_until_garden_number_string;
		if (!TryReadingAndDecryptingDGFString(inputStream, apply_play_password_until_garden_number_string)) {
			cerr << "Failed to read apply play password until garden number string." << endl;
			return false;
		}
		result.applyPlayPasswordUntilGardenIndex = stoi(apply_play_password_until_garden_number_string);
		if (!result.applyPlayPasswordUntilGardenIndex && (apply_play_password_until_garden_number_string != "0")) {
			cerr << "Failed to parse apply play password until garden number." << endl;
			return false;
		}
	}
	if (!TryReadingDGFString(inputStream, result.authorName)) {
		cerr << "Failed to read author name." << endl;
		return false;
	}
	if (!TryReadingDGFString(inputStream, result.comments)) {
		cerr << "Failed to read comment." << endl;
		return false;
	}
	{
		string garden_one_midi_path;
		if (!TryReadingDGFString(inputStream, garden_one_midi_path)) {
			cerr << "Failed to read garden one MIDI path." << endl;
			return false;
		}
	}
	size_t garden_count;
	if (!TryReadingValue<uint16_t, size_t>(inputStream, garden_count)) {
		cerr << "Failed to read garden count." << endl;
		return false;
	}
	{
		uint8_t unknown_byte;
		if (!TryReadingValue<uint8_t>(inputStream, unknown_byte)) {
			cerr << "Failed to read internal unknown constant." << endl;
			return false;
		}
		switch (unknown_byte) {
		case 0x00:
			// Version 1
			break;
		case 0xFF:
			// Version 2
			array<uint8_t, legacyGardenCollectionRemainingUnknownInternalConstant.size()> remaining_unknown_internal_constant;
			if (!TryReadingArray<uint8_t, remaining_unknown_internal_constant.size()>(inputStream, remaining_unknown_internal_constant) || (remaining_unknown_internal_constant != legacyGardenCollectionRemainingUnknownInternalConstant)) {
				cerr << "Failed to read internal unknown constant." << endl;
				return false;
			}
			break;
		default:
			cerr << "Failed to parse internal unknown constant." << endl;
			return false;
		}
	}
	{
		string internal_header;
		if (!TryReadingRawString(inputStream, legacyGardenCollectionInternalHeaderString.size(), internal_header) || (internal_header != legacyGardenCollectionInternalHeaderString)) {
			cerr << "Failed to read internal unknown constant." << endl;
			return false;
		}
	}
	{
		uint16_t program_version;
		if (!TryReadingValue<uint16_t>(inputStream, program_version)) {
			cerr << "Failed to read program version." << endl;
			return false;
		}
		if ((program_version != 1) && (program_version != 2)) {
			cerr << "Invalid parsed program version \"" << program_version << "\"." << endl;
			return false;
		}
	}
	result.gardenCollection.resize(garden_count);
	for (size_t garden_index(static_cast<size_t>(0U)); garden_index != garden_count; garden_index++) {
		if (garden_index != static_cast<size_t>(0U)) {
			uint32_t garden_separator;
			if (!TryReadingValue<uint32_t>(inputStream, garden_separator) || (garden_separator != legacyGardenCollectionGardenSeparator)) {
				cerr << "Failed to read garden separator at garden index \"" << garden_index << "\"." << endl;
				return false;
			}
		}
		GardenData& garden_data(result.gardenCollection.at(static_cast<size_t>(garden_index)));
		if (!TryReadingDGFString(inputStream, garden_data.name)) {
			cerr << "Failed to read garden name at garden index \"" << garden_index << "\"." << endl;
			return false;
		}
		if (!TryReadingDGFString(inputStream, garden_data.midiPath)) {
			cerr << "Failed to read garden MIDI path at garden index \"" << garden_index << "\"." << endl;
			return false;
		}
		{
			Vector2<size_t> garden_size;
			if (!TryReadingValue<uint16_t, size_t>(inputStream, garden_size.x)) {
				cerr << "Failed to read garden width at garden index \"" << garden_index << "\"." << endl;
				return false;
			}
			if (!TryReadingValue<uint16_t, size_t>(inputStream, garden_size.y)) {
				cerr << "Failed to read garden height at garden index \"" << garden_index << "\"." << endl;
				return false;
			}
			garden_data.cells.Resize(garden_size, EGridResizingRule::Naive);
		}
		if (!TryReadingValue<uint16_t, size_t>(inputStream, garden_data.timeInGameSeconds)) {
			cerr << "Failed to read garden time at garden index \"" << garden_index << "\"." << endl;
			return false;
		}
		{
			size_t grid_cell_count;
			if (!TryReadingValue<uint32_t, size_t>(inputStream, grid_cell_count)) {
				cerr << "Failed to read garden grid cell count at garden index \"" << garden_index << "\"." << endl;
				return false;
			}
			if (grid_cell_count != (garden_data.cells.GetCells().size())) {
				cerr << "Garden grid cell count \"" << grid_cell_count << "\" does not match garden size \"" << garden_data.cells.GetSize().x << " x " << garden_data.cells.GetSize().y << " = " << garden_data.cells.GetCells().size() << "\" at garden index \"" << garden_index << "\"." << endl;
				return false;
			}
			for (size_t cell_index(static_cast<size_t>(0U)); cell_index != grid_cell_count; cell_index++) {
				ELegacyGardenCellType cell_type;
				if (!TryReadingValue<ELegacyGardenCellType>(inputStream, cell_type)) {
					cerr << "Failed to read cell type at cell index \"" << cell_index << "\" and garden index \"" << garden_index << "\"." << endl;
					return false;
				}
				uint16_t cell_type_variant;
				if (!TryReadingValue<uint16_t>(inputStream, cell_type_variant)) {
					cerr << "Failed to read cell type variant at cell index \"" << cell_index << "\" and garden index \"" << garden_index << "\"." << endl;
					return false;
				}
				EGardenCellType cell;
				switch (cell_type) {
				case ELegacyGardenCellType::Air:
					switch (static_cast<ELegacyGardenAirCellTypeVariant>(cell_type_variant)) {
					case ELegacyGardenAirCellTypeVariant::Nothing:
						cell = EGardenCellType::Air;
						break;
					case ELegacyGardenAirCellTypeVariant::Fence:
						cell = EGardenCellType::Fence;
						break;
					case ELegacyGardenAirCellTypeVariant::Bush:
						cell = EGardenCellType::Bush;
						break;
					case ELegacyGardenAirCellTypeVariant::LeftCloud:
						cell = EGardenCellType::LeftCloud;
						break;
					case ELegacyGardenAirCellTypeVariant::RightCloud:
						cell = EGardenCellType::RightCloud;
						break;
					case ELegacyGardenAirCellTypeVariant::Sun:
						cell = EGardenCellType::Sun;
						break;
					case ELegacyGardenAirCellTypeVariant::LeftArrow:
						cell = EGardenCellType::LeftArrow;
						break;
					case ELegacyGardenAirCellTypeVariant::RightArrow:
						cell = EGardenCellType::RightArrow;
						break;
					case ELegacyGardenAirCellTypeVariant::UpArrow:
						cell = EGardenCellType::UpArrow;
						break;
					case ELegacyGardenAirCellTypeVariant::DownArrow:
						cell = EGardenCellType::DownArrow;
						break;
					default:
						cerr << "Unknown air cell type variant \"" << cell_type_variant << "\" at cell index \"" << cell_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					break;
				case ELegacyGardenCellType::Ground:
					switch (static_cast<ELegacyGardenGroundCellTypeVariant>(cell_type_variant))
					{
					case ELegacyGardenGroundCellTypeVariant::Default:
						cell = EGardenCellType::Ground;
						break;
					case ELegacyGardenGroundCellTypeVariant::Trap:
						cell = EGardenCellType::Trap;
						break;
					case ELegacyGardenGroundCellTypeVariant::HardGround:
						cell = EGardenCellType::HardGround;
						break;
					case ELegacyGardenGroundCellTypeVariant::GrassGround:
						cell = EGardenCellType::GrassGround;
						break;
					case ELegacyGardenGroundCellTypeVariant::GrassTrap:
						cell = EGardenCellType::GrassTrap;
						break;
					case ELegacyGardenGroundCellTypeVariant::HardGrassGround:
						cell = EGardenCellType::HardGrassGround;
						break;
					case ELegacyGardenGroundCellTypeVariant::MoleHill:
						cell = EGardenCellType::MoleHill;
						break;
					default:
						cerr << "Unknown ground cell type variant \"" << cell_type_variant << "\" at cell index \"" << cell_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					break;
				case ELegacyGardenCellType::Climbable:
					switch (static_cast<ELegacyGardenClimbableCellTypeVariant>(cell_type_variant)) {
					case ELegacyGardenClimbableCellTypeVariant::Ladder:
						cell = EGardenCellType::Ladder;
						break;
					case ELegacyGardenClimbableCellTypeVariant::LadderShowingUpWhenFinished:
						cell = EGardenCellType::LadderShowingUpWhenFinished;
						break;
					case ELegacyGardenClimbableCellTypeVariant::Rope:
						cell = EGardenCellType::Rope;
						break;
					case ELegacyGardenClimbableCellTypeVariant::LadderVanishingWhenFinished:
						cell = EGardenCellType::LadderVanishingWhenFinished;
						break;
					default:
						cerr << "Unknown climbable cell type variant \"" << cell_type_variant << "\" at cell index \"" << cell_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					break;
				case ELegacyGardenCellType::Door:
					switch (static_cast<ELegacyGardenDoorCellTypeVariant>(cell_type_variant)) {
					case ELegacyGardenDoorCellTypeVariant::RedDoor:
						cell = EGardenCellType::RedDoor;
						break;
					case ELegacyGardenDoorCellTypeVariant::YellowDoor:
						cell = EGardenCellType::YellowDoor;
						break;
					case ELegacyGardenDoorCellTypeVariant::GreenDoor:
						cell = EGardenCellType::GreenDoor;
						break;
					case ELegacyGardenDoorCellTypeVariant::AutomaticallyClosingRedDoor:
						cell = EGardenCellType::AutomaticallyClosingRedDoor;
						break;
					case ELegacyGardenDoorCellTypeVariant::AutomaticallyClosingYellowDoor:
						cell = EGardenCellType::AutomaticallyClosingYellowDoor;
						break;
					case ELegacyGardenDoorCellTypeVariant::AutomaticallyClosingGreenDoor:
						cell = EGardenCellType::AutomaticallyClosingGreenDoor;
						break;
					default:
						cerr << "Unknown door cell type variant \"" << cell_type_variant << "\" at cell index \"" << cell_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					break;
				case ELegacyGardenCellType::Water:
					switch (static_cast<ELegacyGardenWaterCellTypeVariant>(cell_type_variant)) {
					case ELegacyGardenWaterCellTypeVariant::Full:
						cell = EGardenCellType::Water;
						break;
					case ELegacyGardenWaterCellTypeVariant::Surface:
						cell = EGardenCellType::SurfaceWater;
						break;
					default:
						cerr << "Unknown water cell type variant \"" << cell_type_variant << "\" at cell index \"" << cell_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					break;
				default:
					cerr << "Unknown cell type \"" << static_cast<uint8_t>(cell_type) << "\" at cell index \"" << cell_index << "\" and garden index \"" << garden_index << "\"." << endl;
					return false;
				}
				Vector2<size_t> cell_position(cell_index % garden_data.cells.GetSize().x, cell_index / garden_data.cells.GetSize().x);
				cell_position.y = garden_data.cells.GetSize().y - cell_position.y - static_cast<size_t>(1U);
				garden_data.cells.SetCell(cell_position, cell);
			}
		}
		{
			size_t entity_count;
			if (!TryReadingValue<uint32_t, size_t>(inputStream, entity_count)) {
				cerr << "Failed to read garden entity count at garden index \"" << garden_index << "\"." << endl;
				return false;
			}
			garden_data.entities.resize(entity_count);
			for (size_t entity_index(static_cast<size_t>(0U)); entity_index != entity_count; entity_index++) {
				ELegacyGardenEntityType entity_type;
				if (!TryReadingValue<ELegacyGardenEntityType>(inputStream, entity_type)) {
					cerr << "Failed to read entity type at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
					return false;
				}
				uint16_t entity_type_variant;
				if (!TryReadingValue<uint16_t>(inputStream, entity_type_variant)) {
					cerr << "Failed to read entity type variant at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
					return false;
				}
				GardenEntityData& entity_data(garden_data.entities.at(entity_index));
				if (!TryReadingValue<uint16_t, size_t>(inputStream, entity_data.position.x)) {
					cerr << "Failed to read entity X position component at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
					return false;
				}
				if (!TryReadingValueAndFixingYPosition<uint16_t, size_t>(inputStream, garden_data.cells.GetSize().y, entity_data.position.y)) {
					cerr << "Failed to read entity Y position component at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
					return false;
				}
				switch (entity_type) {
				case ELegacyGardenEntityType::Player:
					if (entity_type_variant != 0) {
						cerr << "Invalid player entity type variant \"" << entity_type_variant << "\" at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					entity_data.type = EGardenEntityType::Player;
					break;
				case ELegacyGardenEntityType::Marmot:
					if (entity_type_variant != 0) {
						cerr << "Invalid player entity type variant \"" << entity_type_variant << "\" at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					entity_data.type = EGardenEntityType::Marmot;
					if (!TryReadingBounds<int16_t, int64_t>(inputStream, entity_data.bounds)) {
						cerr << "Failed to read marmot entity bounds at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					entity_data.bounds.left = -entity_data.bounds.left;
					entity_data.bounds.bottom = -entity_data.bounds.bottom;
					entity_data.bounds += entity_data.position.GetConverted<int64_t>();
					break;
				case ELegacyGardenEntityType::Worm:
					switch (static_cast<ELegacyGardenWormEntityVariantType>(entity_type_variant)) {
					case ELegacyGardenWormEntityVariantType::RightMovingWorm:
						entity_data.type = EGardenEntityType::RightMovingWorm;
						break;
					case ELegacyGardenWormEntityVariantType::LeftMovingWorm:
						entity_data.type = EGardenEntityType::LeftMovingWorm;
						break;
					default:
						cerr << "Invalid worm entity type variant \"" << entity_type_variant << "\" at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					if (!TryReadingBounds<int16_t, int64_t>(inputStream, entity_data.bounds)) {
						cerr << "Failed to read worm entity bounds at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					entity_data.bounds.left = -entity_data.bounds.left;
					entity_data.bounds.bottom = -entity_data.bounds.bottom;
					entity_data.bounds += entity_data.position.GetConverted<int64_t>();
					break;
				case ELegacyGardenEntityType::Lift:
					switch (static_cast<ELegacyGardenLiftEntityVariantType>(entity_type_variant)) {
					case ELegacyGardenLiftEntityVariantType::UpMovingLift:
						entity_data.type = EGardenEntityType::UpMovingLift;
						break;
					case ELegacyGardenLiftEntityVariantType::DownMovingLift:
						entity_data.type = EGardenEntityType::DownMovingLift;
						break;
					case ELegacyGardenLiftEntityVariantType::LeftMovingLift:
						entity_data.type = EGardenEntityType::LeftMovingLift;
						break;
					case ELegacyGardenLiftEntityVariantType::RightMovingLift:
						entity_data.type = EGardenEntityType::RightMovingLift;
						break;
					default:
						cerr << "Invalid lift entity type variant \"" << entity_type_variant << "\" at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					if (!TryReadingBounds<int16_t, int64_t>(inputStream, entity_data.bounds)) {
						cerr << "Failed to read lift entity bounds at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					entity_data.bounds.left = -entity_data.bounds.left;
					entity_data.bounds.bottom = -entity_data.bounds.bottom;
					entity_data.bounds += entity_data.position.GetConverted<int64_t>();
					break;
				case ELegacyGardenEntityType::Key:
					switch (static_cast<ELegacyGardenKeyEntityVariantType>(entity_type_variant)) {
					case ELegacyGardenKeyEntityVariantType::RedKey:
						entity_data.type = EGardenEntityType::RedKey;
						break;
					case ELegacyGardenKeyEntityVariantType::YellowKey:
						entity_data.type = EGardenEntityType::YellowKey;
						break;
					case ELegacyGardenKeyEntityVariantType::GreenKey:
						entity_data.type = EGardenEntityType::GreenKey;
						break;
					default:
						cerr << "Invalid key entity type variant \"" << entity_type_variant << "\" at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					break;
				case ELegacyGardenEntityType::Fruit:
					switch (static_cast<ELegacyGardenFruitEntityVariantType>(entity_type_variant)) {
					case ELegacyGardenFruitEntityVariantType::Apple:
						entity_data.type = EGardenEntityType::Apple;
						break;
					case ELegacyGardenFruitEntityVariantType::Lemon:
						entity_data.type = EGardenEntityType::Lemon;
						break;
					case ELegacyGardenFruitEntityVariantType::Cherry:
						entity_data.type = EGardenEntityType::Cherry;
						break;
					case ELegacyGardenFruitEntityVariantType::Pineapple:
						entity_data.type = EGardenEntityType::Pineapple;
						break;
					default:
						cerr << "Invalid fruit entity type variant \"" << entity_type_variant << "\" at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					break;
				case ELegacyGardenEntityType::Vegetable:
					switch (static_cast<ELegacyGardenVegetableEntityVariantType>(entity_type_variant)) {
					case ELegacyGardenVegetableEntityVariantType::Garlic:
						entity_data.type = EGardenEntityType::Garlic;
						break;
					case ELegacyGardenVegetableEntityVariantType::Mushroom:
						entity_data.type = EGardenEntityType::Mushroom;
						break;
					case ELegacyGardenVegetableEntityVariantType::Spinach:
						entity_data.type = EGardenEntityType::Spinach;
						break;
					case ELegacyGardenVegetableEntityVariantType::Carrot:
						entity_data.type = EGardenEntityType::Carrot;
						break;
					default:
						cerr << "Invalid vegetable entity type variant \"" << entity_type_variant << "\" at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					break;
				case ELegacyGardenEntityType::Flower:
					switch (static_cast<ELegacyGardenFlowerEntityVariantType>(entity_type_variant)) {
					case ELegacyGardenFlowerEntityVariantType::Sunflower:
						entity_data.type = EGardenEntityType::Sunflower;
						break;
					case ELegacyGardenFlowerEntityVariantType::Tulip:
						entity_data.type = EGardenEntityType::Tulip;
						break;
					case ELegacyGardenFlowerEntityVariantType::YellowDaisy:
						entity_data.type = EGardenEntityType::YellowDaisy;
						break;
					case ELegacyGardenFlowerEntityVariantType::Rose:
						entity_data.type = EGardenEntityType::Rose;
						break;
					default:
						cerr << "Invalid flower entity type variant \"" << entity_type_variant << "\" at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					break;
				case ELegacyGardenEntityType::QuestionMark:
					entity_data.type = EGardenEntityType::QuestionMark;
					if (!TryReadingDGFString(inputStream, entity_data.text)) {
						cerr << "Failed to read question mark entity text at entity index \"" << entity_index << "\" and garden index \"" << garden_index << "\"." << endl;
						return false;
					}
					break;
				}
			}
		}
	}
	return true;
}

bool LegacyGardenCollectionDeserializer::TryReadingRawString(istream& inputStream, size_t stringLength, string& result) noexcept {
	result.resize(stringLength, '\0');
	bool ret(inputStream.read(reinterpret_cast<char*>(result.data()), static_cast<streamsize>(stringLength) * sizeof(string::value_type)).good());
	if (!ret) {
		result.clear();
	}
	return ret;
}

bool LegacyGardenCollectionDeserializer::TryReadingDGFString(istream& inputStream, string& result) noexcept {
	bool ret(false);
	result.clear();
	if (inputStream.good()) {
		size_t dgf_string_length(static_cast<size_t>(inputStream.get()));
		ret =
			((dgf_string_length != 0xFF) || TryReadingValue<uint16_t, size_t>(inputStream, dgf_string_length)) &&
			TryReadingRawString(inputStream, static_cast<size_t>(dgf_string_length), result);
	}
	return ret;
}

bool LegacyGardenCollectionDeserializer::TryReadingAndDecryptingDGFString(istream& inputStream, string& result) noexcept {
	bool ret(false);
	string encrypted_string;
	if (TryReadingDGFString(inputStream, encrypted_string) && (encrypted_string.length() % static_cast<size_t>(2U)) == static_cast<size_t>(0U)) {
		result.resize(encrypted_string.length() / static_cast<size_t>(2U));
		ret = true;
		uint8_t previous_input_byte(static_cast<uint8_t>(0xCB));
		for (size_t index(static_cast<size_t>(0U)); index != result.length(); index++) {
			string::value_type msb_char(encrypted_string.at(index * static_cast<size_t>(2U)));
			string::value_type lsb_char(encrypted_string.at((index * static_cast<size_t>(2U)) + static_cast<size_t>(1U)));
			uint8_t input_byte(static_cast<uint8_t>(0U));
			if ((msb_char >= '0') && (msb_char <= '9')) {
				input_byte |= static_cast<uint8_t>(msb_char - '0') << static_cast<uint8_t>(4U);
			}
			else if ((msb_char >= 'A') && (msb_char <= 'F')) {
				input_byte |= (static_cast<uint8_t>(msb_char - 'A') + static_cast<uint8_t>(10U)) << static_cast<uint8_t>(4U);
			}
			else if ((msb_char >= 'a') && (msb_char <= 'f')) {
				input_byte |= (static_cast<uint8_t>(msb_char - 'a') + static_cast<uint8_t>(10U)) << static_cast<uint8_t>(4U);
			}
			else {
				ret = false;
				result.clear();
				break;
			}
			if ((lsb_char >= '0') && (lsb_char <= '9')) {
				input_byte |= static_cast<uint8_t>(lsb_char - '0');
			}
			else if ((lsb_char >= 'A') && (lsb_char <= 'F')) {
				input_byte |= static_cast<uint8_t>(lsb_char - 'A') + static_cast<uint8_t>(10U);
			}
			else if ((lsb_char >= 'a') && (lsb_char <= 'f')) {
				input_byte |= static_cast<uint8_t>(lsb_char - 'a') + static_cast<uint8_t>(10U);
			}
			else {
				ret = false;
				result.clear();
				break;
			}
			result[index] = previous_input_byte ^ input_byte;
			previous_input_byte = input_byte;
		}
	}
	return ret;
}
