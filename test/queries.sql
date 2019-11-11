USE database1;
INSERT INTO user_notes (id, user_id, note, created) VALUES (1, 1, "Note 1", NOW());
SELECT id, name, address FROM users WHERE is_customer IS NOT NULL ORDER BY created;
DELETE FROM database2.logs WHERE id < 1000;
SELECT filed1, filed2_and_half FROM _database1.__table_weired WHERE filed1 > 10;
SELECT name, last_name FROM db1.users WHERE name = "Soroush" order by last_name;
SELECT text_field FROM text_table WHERE text_field = "Unable to resolve host";
