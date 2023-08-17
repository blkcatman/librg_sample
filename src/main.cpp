#include <iostream>
#include <random>
#include <iomanip>

#define LIBRG_IMPL
#include "librg.h"

template<typename TYPE, std::size_t SIZE> std::size_t array_length(const TYPE (&)[SIZE])
{   
    return SIZE;
}

int main()
{
    /* create a world */

    librg_world *world = librg_world_create();

    if (librg_world_valid(world) == LIBRG_FALSE) {
        std::cout << "Failed to create a world" << std::endl;
        return 1;
    }

    std::cout << "A world is created\n" << std::endl;

    const int chunksize = 16;
    const int cuunkamount = 16;

    librg_config_chunksize_set(world, chunksize, chunksize, 1);
    librg_config_chunkamount_set(world, cuunkamount, cuunkamount, 1);
    librg_config_chunkoffset_set(world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID);

    /* create owner and set position */

    double init_pos_x = 8.0;
    double init_pos_y = 8.0;
    double init_pos_z = 0.0;

    int64_t own_entity_id = 1;
    int64_t owner_id = 1;

    librg_chunk initial_chunk = librg_chunk_from_realpos(
        world,
        init_pos_x,
        init_pos_y,
        init_pos_z
    );

    std::cout << "initial chunk_id:" << initial_chunk << ", ";
    std::cout << "pos: (" << std::fixed << std::setprecision(3) << init_pos_x << ", " <<  init_pos_y << ")\n" << std::endl;

    librg_entity_track(world, own_entity_id);
    librg_entity_owner_set(world, own_entity_id, owner_id);
    librg_entity_chunk_set(world, own_entity_id, initial_chunk);

    /* create entities and set random position */

    std::random_device rd_device;
    std::default_random_engine rd_engine(rd_device());
    std::uniform_real_distribution<double> rd_dist(
        -1.0 * chunksize * (cuunkamount / 2),
        1.0 * chunksize * (cuunkamount / 2)
    );

    int entity_sum = 10000;
    double buf_pos_x[1024];
    double buf_pos_y[1024];

    for(int i = 0; i < 1024; i++) {
        double pos_x = rd_dist(rd_engine);
        double pos_y = rd_dist(rd_engine);
        double pos_z = 0.0;
        buf_pos_x[i] = pos_x;
        buf_pos_y[i] = pos_y;

        int64_t entity_id = i + entity_sum;
        librg_chunk chunk = librg_chunk_from_realpos(world, pos_x, pos_y, pos_z);
        librg_entity_track(world, entity_id);
        librg_entity_chunk_set(world, entity_id, chunk);
    }

    /* query entities on initial position */

    uint8_t target_radius = 1;
    int64_t found_entity_ids[64];
    size_t found_entity_ids_size = array_length(found_entity_ids);

    librg_world_query(world, owner_id, target_radius, found_entity_ids, &found_entity_ids_size);
    std::cout << "query result(n=" << found_entity_ids_size << "):" << std::endl;

    for(int i = 0; i < found_entity_ids_size; i++)
    {
        int64_t entity_id = found_entity_ids[i];
        if (entity_id == own_entity_id) continue;

        int64_t buf_id = entity_id - entity_sum;
        double pos_x = buf_pos_x[buf_id];
        double pos_y = buf_pos_y[buf_id];
        std::cout << "entity_id:" << entity_id << ", ";
        std::cout << "chunk_id:" << librg_entity_chunk_get(world, entity_id) << ", ";
        std::cout << "pos(" << std::fixed << std::setprecision(3) << pos_x << ", " <<  pos_y << "), ";
        std::cout << "dist.(" << std::fixed << std::setprecision(3) << pos_x - init_pos_x << ", " <<  pos_y - init_pos_y << ")\n";
    }
    std::cout << std::endl;

    /* query entities on updated position */
    double update_pos_x = 32.0;
    double update_pos_y = 32.0;
    double update_pos_z = 0.0;

    librg_chunk update_chunk = librg_chunk_from_realpos(
        world,
        update_pos_x,
        update_pos_y,
        update_pos_z
    );

    std::cout << "updated chunk_id:" << update_chunk << ", ";
    std::cout << "pos: (" << std::fixed << std::setprecision(3) << update_pos_x << ", " <<  update_pos_y << ")\n" << std::endl;

    librg_entity_chunk_set(world, own_entity_id, update_chunk);

    librg_world_query(world, owner_id, target_radius, found_entity_ids, &found_entity_ids_size);
    std::cout << "query result(n=" << found_entity_ids_size << "):" << std::endl;

    for(int i = 0; i < found_entity_ids_size; i++)
    {
        int64_t entity_id = found_entity_ids[i];
        if (entity_id == own_entity_id) continue;

        int64_t buf_id = entity_id - entity_sum;
        double pos_x = buf_pos_x[buf_id];
        double pos_y = buf_pos_y[buf_id];
        std::cout << "entity_id:" << entity_id << ", ";
        std::cout << "chunk_id:" << librg_entity_chunk_get(world, entity_id) << ", ";
        std::cout << "(" << std::fixed << std::setprecision(3) << pos_x << ", " <<  pos_y << "), ";
        std::cout << "dist.(" << std::fixed << std::setprecision(3) << pos_x - update_pos_x << ", " <<  pos_y - update_pos_y << ")\n";
    }
    std::cout << std::endl;

    /* cleanup before application quit */

    for(int i = 0; i < 1024; i++) {
        int64_t entity_id = i + entity_sum;
        librg_entity_untrack(world, entity_id);
    }

    librg_world_destroy(world);

    return 0;
}
