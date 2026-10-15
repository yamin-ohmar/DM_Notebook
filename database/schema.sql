CREATE TABLE IF NOT EXISTS npcs (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    race TEXT DEFAULT 'Unknown',
    role TEXT DEFAULT 'Unknown',
    description TEXT DEFAULT 'None'
);